/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:56:31 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/07 13:52:07 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//---------------------JOIN-------------------//

void	create_oa_join(std::string name_chann, Commands *cmd, Server *s, Messages *m, Client *user, std::string key)
{
	std::string	msg;
	
	if (cmd->chanExist(name_chann) == false)
	{
		Channel	*chan;
		
		if (!key.empty())
			chan = new Channel(name_chann, user, key);
		else
			chan = new Channel(name_chann, user);
		s->addListChan(chan);
	}
	else
	{
		if (!s->getChannel(name_chann)->getKey().empty() && key != s->getChannel(name_chann)->getKey())
			m->ERR_CANNOTJOIN(user->get_fd(), name_chann, 475);
		else if (s->getChannel(name_chann)->getSizeMax() != -1 && s->getChannel(name_chann)->nbUserInChan() >= s->getChannel(name_chann)->getSizeMax())
			m->ERR_CANNOTJOIN(user->get_fd(), name_chann, 471);
		else if (s->getChannel(name_chann)->isBanned(user->getNickname()))
			m->ERR_CANNOTJOIN(user->get_fd(), name_chann, 474);
		else
			s->getChannel(name_chann)->addUser(user, user->get_fd());
	}
	if (!s->getChannel(name_chann)->userIsInChann(user->get_fd()))
		return ;
	Channel*	c = s->getChannel(name_chann);
	
	msg = ":" + user->getNickname() + " JOIN " + name_chann + "\r\n";
	c->sendMsg(-1, msg);
	if (c->getTopic().size() < 1)
		m->RPL_NOTOPIC(c, user->get_fd(), user->getNickname());
	else
		m->RPL_TOPIC(c, user->get_fd(), user->getNickname());
	m->RPL_NAMREPLY(c, user->get_fd());
	m->RPL_ENDOFNAMES(c, user->get_fd(), user->getNickname());
}

void	Commands::joinCmd()
{
	bool	key_empty = true;

	if (!this->_line_cmd[2].empty())
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
					create_oa_join(list_chan[i], this, this->_s, this->_msg, this->_user, list_key[i]);
				else
					create_oa_join(list_chan[i], this, this->_s, this->_msg, this->_user, "");
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
			{
				create_oa_join(this->_line_cmd[1], this, this->_s, this->_msg, this->_user, this->_line_cmd[2]);
			}
			else
				create_oa_join(this->_line_cmd[1], this, this->_s, this->_msg, this->_user, "");
		}
	}
}
