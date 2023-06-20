/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:54:12 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/20 10:23:09 by clecat           ###   ########.fr       */
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
	std::cout << "1" << std::endl;
	if (this->_line_cmd.size() < 3){
		std::cout << "2" << std::endl;
		_msg->ERR_NORECIPIENT(this->_fd_user); 
	}
	else if(this->_line_cmd.size() > 2)
	{
		std::cout << "3" << std::endl;
		if ((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&')) // POUR CHANNEL
		{
			std::cout << "4" << std::endl;
			if (chanExist(this->_line_cmd[1]) == true && this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true 
				&& this->_s->getChannel(this->_line_cmd[1])->isBanned(this->_user->getNickname()) == false)
			{
				std::cout << "5" << std::endl;
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
				std::cout << "6" << std::endl;
				this->_msg->ERR_CANNOTSENDTOCHAN(this->_line_cmd[1], this->_fd_user);
				return ;
			}
		}
		else if (this->_s->clientExist(this->_line_cmd[1]) == true) // POUR USER
		{
			std::cout << "7" << std::endl;
			if (joinMessages(2) == ": ")
			{
				std::cout << "10" << std::endl;
				this->_msg->ERR_NOTEXTTOSEND(this->_fd_user);
				return ;
			}
			else{
				std::cout << "11" << std::endl;
				this->_msg->RPL_PRIVMSG(this->_user->getNickname(), this->_line_cmd[1], joinMessages(2), this->_s->getClient(this->_line_cmd[1])->get_fd());
				//segfault pour /dcc en sortie de RPL_PRIVMSG
			}
		}
		else{
			std::cout << "8" << std::endl;
			this->_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
		}
		if (this->_s->getChannel(this->_line_cmd[1])->getListUserCo().empty())
		{
			std::cout << "9" << std::endl;
       		this->_s->channDisconnected();
		}
	}
}