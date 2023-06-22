/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:56:31 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 08:32:38 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
/*
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_TOOMANYCHANNELS (405)
	ERR_BADCHANNELKEY (475)
	ERR_BANNEDFROMCHAN (474)
	ERR_CHANNELISFULL (471)
	ERR_INVITEONLYCHAN (473)
	ERR_BADCHANMASK (476)
	RPL_TOPIC (332)
	RPL_TOPICWHOTIME (333)
	RPL_NAMREPLY (353)
	RPL_ENDOFNAMES (366)
*/
// /connect localhost 8080 oui
//---------------------JOIN-------------------//

void	Commands::create_oa_join(std::string name_chann, std::string key)
{
	std::string	msg;
	
	bool		chanExist = this->chanExist(name_chann);
	
	if (!chanExist)
	{
		Channel	*chan;
		
		if (!key.empty())
			chan = new Channel(name_chann, this->_user, key);
		else
			chan = new Channel(name_chann, this->_user);
		this->_s->addListChan(chan);
		std::cout << " >> " << YELLOW << "Channel named " << name_chann << " has been created by " << this->_s->getChannel(name_chann)->getCreator()->getNickname() << Color << std::endl;
	}
	else
	{
		if (!this->_s->getChannel(name_chann)->getKey().empty() && key != this->_s->getChannel(name_chann)->getKey())
			this->_msg->ERR_CANNOTJOIN(this->_user->get_fd(), name_chann, 475);
		else if (this->_s->getChannel(name_chann)->getSizeMax() != -1 && this->_s->getChannel(name_chann)->nbUserInChan() >= this->_s->getChannel(name_chann)->getSizeMax())
			this->_msg->ERR_CANNOTJOIN(this->_user->get_fd(), name_chann, 471);
		else if (this->_s->getChannel(name_chann)->isBanned(this->_user->getNickname()))
			this->_msg->ERR_CANNOTJOIN(this->_user->get_fd(), name_chann, 474);
		else if (this->_s->getChannel(name_chann)->getInviteOnly() == true && this->_s->getChannel(name_chann)->isInv(this->_user->getNickname()) == false)
			this->_msg->ERR_CANNOTJOIN(this->_user->get_fd(), name_chann, 473);
		else
		{
			this->_s->getChannel(name_chann)->addUser(this->_user, this->_fd_user);
			std::cout << " >> " << YELLOW << this->_user->getNickname() << " has join " << name_chann << Color <<"\n";
		}
	}
	if (!this->_s->getChannel(name_chann)->userIsInChann(this->_user->get_fd()))
		return ;
	Channel*	c = this->_s->getChannel(name_chann);
	
	this->_msg->RPL_JOIN(this->_user->getNickname(), this->_user->getUser(), name_chann, c);
	if (!c->topicIsSet())
		this->_msg->RPL_NOTOPIC(c);
	else
		this->_msg->RPL_TOPIC(c);
	this->_msg->RPL_NAMREPLY(c, this->_user->get_fd());
	this->_msg->RPL_ENDOFNAMES(c, this->_user->get_fd(), this->_user->getNickname());
}


void	Commands::joinCmd()
{
	bool	key_empty = true;

	if (this->_line_cmd.size() > 2 && this->_line_cmd[2].empty() != 1)
			key_empty = false;
	if (this->_line_cmd.size() == 1)
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
	if (this->_line_cmd[1].find(',') != std::string::npos)
	{
		std::vector<std::string>	list_chan = this->_s->splitCustom(this->_line_cmd[1], ',');
		std::vector<std::string>	list_key = this->_s->splitCustom(this->_line_cmd[2], ',');
		
		unsigned long long			i = 0;
		while (i < list_chan.size())
		{
			if (list_chan[i][0] != '#')
				this->_msg->ERR_NOSUCHCHANNEL(list_chan[i], this->_fd_user);
			else
			{
				if (!key_empty && !list_key[i].empty())
					create_oa_join(list_chan[i], list_key[i]);
				else
					create_oa_join(list_chan[i], "");
			}
			i++;
		}
	}
	else
	{
		if (this->_line_cmd[1][0] != '#')
			this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
		else
		{
			if (!key_empty)
				create_oa_join(this->_line_cmd[1], this->_line_cmd[2]);
			else
				create_oa_join(this->_line_cmd[1], "");
		}
	}
}

//envoie de fichier : command: /dcc <mot clé> <target> chemin fichier
// pour envoyé : <mot clé> = send, pour recevoir <mot clé> = get 
//message reçu sur page du user : 17:30 [localhost] DCC SEND request sent to newuser: Server.hpp 
//message envoyé au serveur: PRIVMSG newuser :DCC SEND Server.hpp 2130706433 37851 3197
//lien doc: https://irssi.org/documentation/help/dcc/
