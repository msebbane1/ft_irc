/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:49:06 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/17 14:18:56 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//---------------------NICK-------------------//

bool	nicknameIsValid(std::string nick)
{
	for (int i = 0; nick[i]; i++)
	{
		if (i == 0 && (nick[i] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&'))
			return (false);
		if (nick[i] == ' ' || nick[0] == ',' || nick[0] == '?' || nick[0] == '!' 
			|| nick[0] == '@' || nick[0] == '*' || nick[0] == '.')
			return (false);
	}
	return (true);
}

void	Commands::nickCmd()
{
	std::string	msg;

	if (this->_line_cmd.size() == 1 ) 
		this->_msg->ERR_NONICKNAMEGIVEN(this->_fd_user);
	else if(this->_line_cmd.size() >= 2)
	{
		if (this->_s->clientExist(this->_line_cmd[1]))
			this->_msg->ERR_NICKNAMEINUSE(this->_line_cmd[1], this->_fd_user);
		else if (nicknameIsValid(this->_line_cmd[1]) == true)
		{
			if(this->_user->nicknameIsSet())
			{
				std::string	msg = ":" +  _user->getNickname() + "!" + _user->getUser() + "@localhost NICK :" + this->_line_cmd[1] + "\r\n"; // POUR CHANGER NICK
				if(send(this->_fd_user, msg.c_str(), msg.length(), 0) < 0)
					_msg->errorMsg("failed send");
			}
			this->_user->setNickname(this->_line_cmd[1]);
			std::cout << " >> " << GREEN << "Nickname: " << this->_user->getNickname() << Color <<"\n";
		}
		else
			this->_msg->ERR_ERRONEUSNICKNAME(this->_line_cmd[1], this->_fd_user);
	}
}