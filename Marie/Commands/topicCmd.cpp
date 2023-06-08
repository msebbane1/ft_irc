/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:02:11 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/08 16:10:07 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	RPL_NOTOPIC (331)
	RPL_TOPIC (332)
	RPL_TOPICWHOTIME (333)

	Exemples :
	TOPIC #test :New topic          ; Setting the topic on "#test" to "New topic"
  	TOPIC #test :                   ; Clearing the topic on "#test"
  	TOPIC #test                     ; Checking the topic for "#test"

	Parameters: <channel> [<topic>]
*/
void	Commands::topicCmd()
{
	bool userInchan = this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user);
	if (this->_line_cmd.size() < 2)
	{
		_msg->ERR_NEEDMOREPARAMS(_fd_user);
		return ;
	}
	if ((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&') && chanExist(this->_line_cmd[1]) == true)
	{
		if (this->_line_cmd.size() == 2)
		{
			if (userInchan == true)
				std::cout << "bb";
				//this->_msg->RPL_TOPIC(this->_user->getNickname(), this->_user->getUser(), this->_line_cmd[1], _user->get);
		}
			//server->send_reply(client->get_nick(), client->get_user(), "TOPIC", server->get_channel(arg[1])->get_topic(), client->get_fd()); 332
		else if(this->_line_cmd.size() > 2) // a verifier sur nc
		{
			std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
			if(userInchan == true)
			{
				for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
				{
					this->_msg->RPL_LEFTCHANNEL(this->_user->getNickname(), this->_user->getUser(), this->_line_cmd[1], it->first);
				}
				this->_s->getChannel(this->_line_cmd[1])->removeUser(_user->getNickname());
			}
			else
				this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user); // verif sur vrai server le msg exac
			/*
			_s->getChannel(this->_line_cmd[1])->setTopic(joinMessages());
			for (client_map_it it = server->get_channel(arg[1])->get_users().begin(); it != server->get_channel(arg[1])->get_users().end(); it++) 
			{
				string to_send = ":" + it->second->get_nick() + "!" + it->second->get_user() + "@localhost TOPIC #" + arg[1] + " :" + joined + "\r\n";
				ft_send(it->second->get_fd(), to_send.c_str());
			}
			*/
		}
	}
	else
		_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], _fd_user);
}