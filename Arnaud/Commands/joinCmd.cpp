/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:56:31 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/02 17:18:48 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//---------------------JOIN-------------------//

// <canal>{,<canal>} [<clé>{,<clé>}]
void	Commands::joinCmd()
{
	std::string	msg;

	if (this->_line_cmd.size() == 1)
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user); //461
	else if (this->_line_cmd[1][0] != '#' || this->_line_cmd[1][0] != '&')
	{
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
		return;
	}
	if (chanExist(this->_line_cmd[1]) == false)
	{
		Channel	*chan = new Channel(this->_line_cmd[1], this->_user);
		this->_s->addListChan(chan);
	}
	else
		this->_s->getChannel(this->_line_cmd[1])->addUser(this->_user, this->_fd_user);
	Channel*	c = this->_s->getChannel(this->_line_cmd[1]);
	
	msg = ":" + this->_user->getNickname() + " JOIN " + this->_line_cmd[1];
	this->_s->getChannel(this->_line_cmd[1])->sendMsg(this->_fd_user, msg);
	if (c->getTopic().size() < 1)
		this->_msg->RPL_NOTOPIC(this->_s->getChannel(this->_line_cmd[1]));
	else
		this->_msg->RPL_TOPIC(this->_s->getChannel(this->_line_cmd[1]));
	this->_msg->RPL_NAMREPLY(this->_s->getChannel(this->_line_cmd[1]));
	this->_msg->RPL_ENDOFNAMES(this->_s->getChannel(this->_line_cmd[1]));
}
