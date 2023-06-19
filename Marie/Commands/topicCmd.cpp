/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:02:11 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/19 13:05:18 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

// /connect localhost 8080 oui
/*
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	RPL_NOTOPIC (331)
	RPL_TOPIC (332)

	// A FAIRE ??
	RPL_TOPICWHOTIME (333)
	ERR_CHANOPRIVSNEEDED (482)

	Exemples :
	TOPIC #test :New topic          ; Setting the topic on "#test" to "New topic"
  	TOPIC #test :                   ; Clearing the topic on "#test"
  	TOPIC #test                     ; Checking the topic for "#test"

	Parameters: <channel> [<topic>]
*/

void	Commands::topicCmd()
{
	if (this->_line_cmd.size() == 1)
	{
		this->_msg->ERR_NEEDMOREPARAMS(_fd_user);
		return ;
	}
	if ((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&') && chanExist(this->_line_cmd[1]) == false)
	{
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], _fd_user);
		return;
	}
	if (this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user))
	{
		if(this->_line_cmd[2] == "::")
		{
			std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
			for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
			{
				std::string msg = ":" + this->_user->getNickname() + "!" + this->_user->getUser() + "@localhost TOPIC :" + this->_line_cmd[1] + "\r\n";
				if(send(it->second->get_fd(), msg.c_str(), msg.length(), 0) < 0)
					this->_msg->errorMsg("failed send");
			}
			return ;
		}
		if (this->_line_cmd.size() > 2 && this->_line_cmd[2] != "::")
		{
			std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
			for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
			{
				std::string msg = ":" + it->second->getNickname() + "!" + it->second->getUser() + "@localhost TOPIC #" + this->_line_cmd[1] + " :" + joinMessages(2) + "\r\n";
				if(send(it->second->get_fd(), msg.c_str(), msg.length(), 0) < 0)
					_msg->errorMsg("failed send");
			}
			this->_s->getChannel(this->_line_cmd[1])->setTopic(joinMessages(2));
		}
		if(this->_s->getChannel(this->_line_cmd[1])->topicIsSet() && this->_line_cmd[2] != "::")
			this->_msg->RPL_TOPIC(this->_s->getChannel(this->_line_cmd[1]));
		else
			this->_msg->RPL_NOTOPIC(this->_s->getChannel(this->_line_cmd[1]));
	}
	else
		this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
}