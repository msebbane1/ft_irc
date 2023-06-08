/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:07:35 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/08 12:17:12 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	RPL_INVITING (341)
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	ERR_USERONCHANNEL (443)

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
	// a rajouter l'erreur pour le mode
	
	msg = ":irc.com 341 RPL_INVITING " + this->_user->getNickname() + " invites you to " + this->_line_cmd[2] + "\r\n";
	if(send(this->_s->getClient(this->_line_cmd[1])->get_fd(), msg.c_str(), msg.length(), 0) < 0)
		this->_msg->errorMsg("failed send");
	msg = ":irc.com 341 RPL_INVITING Inviting " + this->_line_cmd[1] + " to " + this->_line_cmd[2] + "\r\n";
	if(send(this->_fd_user, msg.c_str(), msg.length(), 0) < 0)
		this->_msg->errorMsg("failed send");
	msg = ":irc.com 341 RPL_INVITING " + this->_user->getNickname() + " invited " + this->_line_cmd[1] + " into the channel.\r\n";
	this->_s->getChannel(this->_line_cmd[2])->sendMsg(-1, msg);
}