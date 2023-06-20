/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:54:12 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/20 14:25:00 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
/connect localhost 8080 oui
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

std::string	Commands::joinMessages(int line)
{
	std::string msg;
	std::vector<std::string>::iterator it = this->_line_cmd.begin() + line;
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
	if (this->_line_cmd.size() < 3)
	{
		_msg->ERR_NORECIPIENT(this->_fd_user); 
	}
	else if(this->_line_cmd.size() > 2)
	{
		if ((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&')) // POUR CHANNEL
		{
			if (chanExist(this->_line_cmd[1]) == true && this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true 
				&& this->_s->getChannel(this->_line_cmd[1])->isBanned(this->_user->getNickname()) == false)
			{
				std::string msg = joinMessages(2);
				if (bot->containBanWord(msg))
				{
					this->_s->getChannel(this->_line_cmd[1])->banUser(this->_user->getNickname());
					std::cout << " >> " << RED << this->_user->getNickname() << " is banned from the bot John Wick :) " << std::endl;
					return ;
				}
				if (joinMessages(2) == ": ")
				{
					this->_msg->ERR_NOTEXTTOSEND(this->_fd_user);
					return ;
				}
				else
					this->_msg->RPL_PRIVMSGCHAN(this->_user->getNickname(), this->_line_cmd[1], msg, _s->getChannel(this->_line_cmd[1]), _fd_user);
			}
			else
			{
				this->_msg->ERR_CANNOTSENDTOCHAN(this->_line_cmd[1], this->_fd_user);
				return ;
			}
		}
		else if (this->_s->clientExist(this->_line_cmd[1]) == true) // POUR USER
		{
			if (joinMessages(2) == ": ")
			{
				this->_msg->ERR_NOTEXTTOSEND(this->_fd_user);
				return ;
			}
			else{
				this->_msg->RPL_PRIVMSG(this->_user->getNickname(), this->_line_cmd[1], joinMessages(2), this->_s->getClient(this->_line_cmd[1])->get_fd());
				return;
			}
		}
		else
		{
			this->_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
		}
		if (chanExist(this->_line_cmd[1]) == true && this->_s->getChannel(this->_line_cmd[1])->getListUserCo().empty()) // probleme ici
		{
       		this->_s->channDisconnected();
		}
		
	}
}