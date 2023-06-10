/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:54:12 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/10 15:45:57 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

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

void	Commands::privMsgCmd()
{
	std::string	msg;

	if (this->_line_cmd.size() <= 2)
		_msg->ERR_NORECIPIENT(this->_fd_user); 
	else if(this->_line_cmd.size() > 2)
	{
		if(this->_line_cmd[1].empty())
			_msg->ERR_NOTEXTTOSEND(this->_fd_user);
		if (this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&')
		{
			if(chanExist(this->_line_cmd[1]) == true)
			{
				//if(this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user))
				//{
					msg = ":" + this->_user->getNickname() + " PRIVMSG " + this->_line_cmd[1] + " " + joinMessages() + "\r\n";
					this->_s->getChannel(this->_line_cmd[1])->sendMsg(this->_fd_user, msg);
				//}
				//else
					//this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user); // ERREUR PLUTOT 404 A CHANGER
				
			}
			else
				this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
		}
		else if(this->_s->clientExist(this->_line_cmd[1]) == true) // et changer ajouter quand c pas un channel
		{
			msg = ":" + this->_user->getNickname() + " PRIVMSG " +  this->_line_cmd[1] + " " + joinMessages() + "\r\n";
			std::cout << "=======" << this->_s->getClient(this->_line_cmd[1])->get_fd() << std::endl;
			send(this->_s->getClient(this->_line_cmd[1])->get_fd(), msg.c_str(), msg.length(), 0);
		}
		else
			_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
	}
}