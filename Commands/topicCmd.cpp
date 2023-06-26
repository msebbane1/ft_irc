/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:02:11 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 11:50:51 by msebbane         ###   ########.fr       */
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
	std::string msg;
	
	if (this->_line_cmd.size() == 1)
	{
		this->_msg->ERR_NEEDMOREPARAMS(_fd_user);
		return ;
	}
	if ((this->_line_cmd[1][0] != '#' || this->_line_cmd[1][0] != '&') && chanExist(this->_line_cmd[1]) == false)
	{
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], _fd_user);
		return;
	}
	//verif si topic protected = true only channelOperator can change topic ERR You're not channel operator
	if (this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) && this->_s->getChannel(this->_line_cmd[1])->getTopicProtected() == true)
	{
		if(this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user) == false)
		{
			this->_msg->ERR_CHANOPRIVSNEEDED(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
			return ;
		}
		else
		{
			if (this->_line_cmd[2] == "::")
			{
				msg = ":" + this->_user->getNickname() + "!" + this->_user->getUser() + "@localhost TOPIC :" + this->_line_cmd[1] + "\r\n";
				this->_s->getChannel(this->_line_cmd[1])->sendMsg(-1, msg);
			}
			else if (this->_line_cmd.size() > 2 && this->_line_cmd[2] != "::")
			{
				msg = ":" + this->_user->getNickname() + "!" + this->_user->getUser() + "@localhost TOPIC " + this->_line_cmd[1] + " :" + joinMessages(2) + "\r\n";
				this->_s->getChannel(this->_line_cmd[1])->sendMsg(-1, msg);
				this->_s->getChannel(this->_line_cmd[1])->setTopic(joinMessages(2));
			}
			if (this->_s->getChannel(this->_line_cmd[1])->topicIsSet() && this->_line_cmd[2] != "::")
				this->_msg->RPL_TOPIC(this->_s->getChannel(this->_line_cmd[1]));
			else
				this->_msg->RPL_NOTOPIC(this->_s->getChannel(this->_line_cmd[1]));
		}
	}
	else if (this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) && this->_s->getChannel(this->_line_cmd[1])->getTopicProtected() == false)
	{
		if (this->_line_cmd[2] == "::")
		{
			msg = ":" + this->_user->getNickname() + "!" + this->_user->getUser() + "@localhost TOPIC :" + this->_line_cmd[1] + "\r\n";
			this->_s->getChannel(this->_line_cmd[1])->sendMsg(-1, msg);
		}
		else if (this->_line_cmd.size() > 2 && this->_line_cmd[2] != "::")
		{
			msg = ":" + this->_user->getNickname() + "!" + this->_user->getUser() + "@localhost TOPIC " + this->_line_cmd[1] + " :" + joinMessages(2) + "\r\n";
			this->_s->getChannel(this->_line_cmd[1])->sendMsg(-1, msg);
			this->_s->getChannel(this->_line_cmd[1])->setTopic(joinMessages(2));
		}
		if (this->_s->getChannel(this->_line_cmd[1])->topicIsSet() && this->_line_cmd[2] != "::")
			this->_msg->RPL_TOPIC(this->_s->getChannel(this->_line_cmd[1]));
		else
			this->_msg->RPL_NOTOPIC(this->_s->getChannel(this->_line_cmd[1]));
	}
	else
		this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
}