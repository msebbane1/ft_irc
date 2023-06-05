/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:56:31 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/05 13:37:13 by asahonet         ###   ########.fr       */
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
		if (key != s->getChannel(name_chann)->getKey())
		{
			m->ERR_BADCHANNELKEY(user->get_fd(), name_chann);
			return ;
		}
		if (s->getChannel(name_chann)->nbUserInChan() == s->getChannel(name_chann)->getSizeMax())
		{
			m->ERR_CHANNELISFULL(user->get_fd(), name_chann);
			return ;
		}
		s->getChannel(name_chann)->addUser(user, user->get_fd());
	}
	Channel*	c = s->getChannel(name_chann);
	
	msg = ":" + user->getNickname() + " JOIN " + name_chann + "\r\n";
	c->sendMsg(-1, msg);
	if (c->getTopic().size() < 1)
		m->RPL_NOTOPIC(c);
	else
		m->RPL_TOPIC(c);
	m->RPL_NAMREPLY(c);
	m->RPL_ENDOFNAMES(c);
}

void	Commands::joinCmd()
{
	bool	is_empty = true;

	if (!this->_line_cmd[2].empty())
			is_empty = false;
	if (this->_line_cmd.size() == 1)
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
	if (this->_line_cmd[1].find(',') != std::string::npos)
	{
		std::vector<std::string>	list_chan = this->_s->splitCustom(this->_line_cmd[1], ',');
		std::vector<std::string>	*list_key = 0;
		
		if (!is_empty)
		 	*list_key = this->_s->splitCustom(this->_line_cmd[2], ',');
		unsigned long long			i = 0;

		std::cout << "1---------" << std::endl;
		while (i < list_chan.size())
		{
			std::cout << "2---------" << std::endl;
			if (list_chan[i][0] != '#')
				this->_msg->ERR_NOSUCHCHANNEL(list_chan[i], this->_fd_user);
			else
			{
				std::cout << "3---------" << std::endl;
				if (!is_empty)
				{
					if (!list_key[i].empty())
						create_oa_join(list_chan[i], this, this->_s, this->_msg, this->_user, list_key[0][i]);
					else
						create_oa_join(list_chan[i], this, this->_s, this->_msg, this->_user, "");
				}
				else
					create_oa_join(list_chan[i], this, this->_s, this->_msg, this->_user, "");
				std::cout << "4---------" << std::endl;
			}
			i++;
		}
		std::cout << "5---------" << std::endl;
	}
	else
	{
		if (this->_line_cmd[1][0] != '#')
		{
			this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
			return;
		}
		if (!is_empty)
			create_oa_join(this->_line_cmd[1], this, this->_s, this->_msg, this->_user, this->_line_cmd[2]);
		else
			create_oa_join(this->_line_cmd[1], this, this->_s, this->_msg, this->_user, "");
	}
}
