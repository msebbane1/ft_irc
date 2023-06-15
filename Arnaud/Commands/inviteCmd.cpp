/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:07:35 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/15 11:44:19 by asahonet         ###   ########.fr       */
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
		this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[2], this->_fd_user);
		return ;
	}
	if (this->_s->getChannel(this->_line_cmd[2])->userIsInChann(this->_s->getClient(this->_line_cmd[1])->get_fd()))
	{
		this->_msg->ERR_USERONCHANNEL(this->_fd_user, this->_line_cmd[1], this->_line_cmd[2]);
		return ;
	}
	if (this->_s->getChannel(this->_line_cmd[2])->isOperator(this->_fd_user))
	{
		this->_s->getChannel(this->_line_cmd[2])->addUser(this->_s->getClient(this->_line_cmd[1]), this->_s->getClient(this->_line_cmd[1])->get_fd());
		this->_s->getChannel(this->_line_cmd[2])->addListInv(this->_s->getClient(this->_line_cmd[1])->getNickname());
		this->_msg->RPL_INVITING(this->_user->getNickname(), this->_user->getUser(), this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_line_cmd[2], this->_fd_user);
		this->_msg->RPL_INVITE(this->_user->getNickname(), this->_user->getUser(), this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_line_cmd[2], this->_s->getClient(this->_line_cmd[1])->get_fd());
	
		msg = ":" + this->_s->getClient(this->_line_cmd[1])->getNickname() + " JOIN " + this->_line_cmd[2] + "\r\n";
		this->_s->getChannel(this->_line_cmd[2])->sendMsg(-1, msg);
		std::string msg2 = ":localhost 353 " + this->_user->getUser() + " = " + this->_line_cmd[2] + " :@" + _user->getNickname() + "\r\n";
        send(this->_user->get_fd(), msg2.c_str(), msg2.length(), 0);
	}
	else
	{
		std::cout << "operator NOT" << std::endl;
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
		return ;
	}
}