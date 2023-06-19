/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:54:12 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/19 10:44:44 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	ERR_NORECIPIENT (411)
	ERR_NOTEXTTOSEND (412)
	ERR_NOSUCHNICK (401)
	ERR_CANNOTSENDTOCHAN (404)
	
	//A FAIRE ?
	ERR_NOSUCHSERVER (402)
	RPL_AWAY (301)

	Parameters: <target>{,<target>} <text to be sent>
*/
//---------------------PRIVMSG-------------------//

std::string	Commands::joinMessages()
{
	std::string msg;
	std::vector<std::string>::iterator it = this->_line_cmd.begin() + 2;
	while (it != this->_line_cmd.end())
	{
		msg += *it;
		msg += " ";
		it++;
	}
	//std::cout << msg << std::endl;
	return(msg);
}

void	Commands::privMsgCmd(Client *bot)
{
	std::string	msg;

	if (this->_line_cmd.size() < 3)
		_msg->ERR_NORECIPIENT(this->_fd_user); 
	else if(this->_line_cmd.size() > 2)
	{
		if ((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&')) // POUR CHANNEL
		{ 
			if (chanExist(this->_line_cmd[1]) == true && this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true 
				&& this->_s->getChannel(this->_line_cmd[1])->isBanned(this->_user->getNickname()) == false)
			{
				if (joinMessages() == ": ")
				{
					this->_msg->ERR_NOTEXTTOSEND(this->_fd_user);
					return ;
				}
				else
				{
					msg = joinMessages();
					if (bot->containBanWord(msg))
					{
						this->_s->getChannel(this->_line_cmd[1])->banUser(this->_user->getNickname());
						std::cout << this->_user->getNickname() << " est kick. Cheh." << std::endl;
						return ;
					}
					this->_msg->RPL_PRIVMSGCHAN(this->_user->getNickname(), this->_line_cmd[1], msg, _s->getChannel(this->_line_cmd[1]), _fd_user);
				}
			}
			else
			{
				this->_msg->ERR_CANNOTSENDTOCHAN(this->_line_cmd[1], this->_fd_user);
				return ;
			}
		}
		else if (this->_s->clientExist(this->_line_cmd[1]) == true) // POUR USER
		{
			if (joinMessages() == ": ")
			{
				this->_msg->ERR_NOTEXTTOSEND(this->_fd_user);
				return ;
			}
			else
				this->_msg->RPL_PRIVMSG(this->_user->getNickname(), this->_line_cmd[1], joinMessages(), this->_s->getClient(this->_line_cmd[1])->get_fd());
		}
		else
			this->_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
	}
}