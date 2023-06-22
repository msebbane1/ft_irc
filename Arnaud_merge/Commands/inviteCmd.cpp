/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:07:35 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/22 08:32:04 by msebbane         ###   ########.fr       */
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
	else if(_s->clientExist(this->_line_cmd[1]) == false)
	{
		this->_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
		return ;
	}
	else if (!chanExist(this->_line_cmd[2]))
	{
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[2], this->_fd_user);
		return ;
	}
	else if (!this->_s->getChannel(this->_line_cmd[2])->userIsInChann(this->_fd_user))
	{
		this->_msg->ERR_NOTONCHANNEL(this->_fd_user, this->_line_cmd[2]);
		return ;
	}
	else if (this->_s->getChannel(this->_line_cmd[2])->userIsInChann(this->_s->getClient(this->_line_cmd[1])->get_fd()))
	{
		this->_msg->ERR_USERONCHANNEL(this->_fd_user, this->_line_cmd[1], this->_line_cmd[2]);
		return ;
	}
	if (this->_s->getChannel(this->_line_cmd[2])->isOperator(this->_fd_user))
	{
		this->_msg->RPL_INVITING(_user->getNickname(), _user->getUser(), this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_line_cmd[2], this->_fd_user);
		this->_msg->RPL_INVITE(_user->getNickname(), _user->getUser(), this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_line_cmd[2], this->_s->getClient(this->_line_cmd[1])->get_fd());
	
		//msg = ":" + this->_s->getClient(this->_line_cmd[1])->getNickname() + " JOIN " + this->_line_cmd[2] + "\r\n";
		//_s->getChannel(this->_line_cmd[2])->sendMsg(-1, msg);
		std::string msg2 = ":localhost 353 " + this->_user->getUser() + " = " + this->_line_cmd[2] + " :@" + this->_user->getNickname() + "\r\n";
        send(this->_user->get_fd(), msg2.c_str(), msg2.length(), 0);
		this->_s->getChannel(this->_line_cmd[2])->addListInv(this->_s->getClient(this->_line_cmd[1])->getNickname());
	}
	else
	{
		std::cout << "operator NOT" << std::endl;
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_user->getNickname(), this->_line_cmd[2], this->_fd_user);
		return ;
	}

}