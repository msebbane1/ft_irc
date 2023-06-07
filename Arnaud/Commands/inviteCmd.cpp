/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:07:35 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/07 11:09:08 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	Commands::inviteCmd()
{
	std::string	msg;
	std::string	msg2, msg3;
	
	if (this->_line_cmd.size() < 3)
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return ;
	}
	if (!chanExist(this->_line_cmd[2]))
	{
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[2], this->_fd_user);
		return ;
	}
	if (!this->_s->getChannel(this->_line_cmd[2])->userIsInChann(this->_fd_user))
	{
		this->_msg->ERR_NOTONCHANNEL(this->_fd_user, this->_line_cmd[2]);
		return ;
	}
	if (this->_s->getChannel(this->_line_cmd[2])->userIsInChann(this->_s->getClient(this->_line_cmd[1])->get_fd()))
	{
		this->_msg->ERR_USERONCHANNEL(this->_fd_user, this->_line_cmd[1], this->_line_cmd[2]);
		return ;
	}
	// a rajouter l'erreur pour le mode
	
	msg = ":irc.com 341 RPL_INVITING " + this->_user->getNickname() + " invites you to " + this->_line_cmd[2] + "\r\n";
	if(send(this->_s->getClient(this->_line_cmd[1])->get_fd(), msg.c_str(), msg.length(), 0) < 0)
		this->_msg->errorMsg("failed send");
	msg2 = ":irc.com 341 RPL_INVITING " + this->_line_cmd[1] + " to " + this->_line_cmd[2] + "\r\n";
	if(send(this->_fd_user, msg2.c_str(), msg2.length(), 0) < 0)
		this->_msg->errorMsg("failed send");
	msg3 = ":irc.com 341 RPL_INVITING " + this->_user->getNickname() + " :" + this->_line_cmd[2] + " invited " + this->_line_cmd[1] + " into the channel.\r\n";
	this->_s->getChannel(this->_line_cmd[2])->sendMsg(-1, msg3);
}