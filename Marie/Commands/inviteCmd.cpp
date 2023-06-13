/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:07:35 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/13 14:50:30 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_USERONCHANNEL (443)
	ERR_CHANOPRIVSNEEDED (482)
	RPL_INVITING (341)

	exemples : INVITE Wiz #foo_bar
	Parameters: <nickname> <channel>

*/
void	Commands::inviteCmd()
{
	std::string	msg;
	
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
	if (_s->getChannel(this->_line_cmd[2])->isOperator(this->_fd_user))
	{
		_s->getChannel(this->_line_cmd[2])->addUser(this->_s->getClient(this->_line_cmd[1]), this->_s->getClient(this->_line_cmd[1])->get_fd());
		_s->getChannel(this->_line_cmd[2])->addListInv(this->_s->getClient(this->_line_cmd[1])->getNickname());
		this->_msg->RPL_INVITING(_user->getNickname(), _user->getUser(), this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_line_cmd[2], _fd_user);
		this->_msg->RPL_INVITE(_user->getNickname(), _user->getUser(), this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_line_cmd[2], this->_s->getClient(this->_line_cmd[1])->get_fd());
	
		msg = ":" + this->_s->getClient(this->_line_cmd[1])->getNickname() + " JOIN " + this->_line_cmd[2] + "\r\n";
		_s->getChannel(this->_line_cmd[2])->sendMsg(-1, msg);
		std::string msg2 = ":localhost 353 " + _user->getUser() + " = " + this->_line_cmd[2] + " :@" + _user->getNickname() + "\r\n";
        send(_user->get_fd(), msg2.c_str(), msg2.length(), 0);
	}
	else
	{
		std::cout << "operator NOT" << std::endl;
		_msg->ERR_CHANOPRIVSNEEDED(this->_user->getNickname(), this->_line_cmd[2], _fd_user);
		return ;
	}

}