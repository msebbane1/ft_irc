/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quitCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:58:17 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/20 11:44:31 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
/*
	 Parameters: [<reason>]
*/
//---------------------QUIT-------------------//

bool	Commands::userOnChannel()
{
	int fd;
	std::map<std::string, Channel *> listChannel = this->_s->getListChan();
	std::map<int, Client*>	listClient = this->_s->getListClient();
	
	for (std::map<int, Client*>::iterator it = listClient.begin(); it != listClient.end(); it++)
	{
		fd = it->first;
		for (std::map<std::string, Channel *>::iterator it = listChannel.begin(); it != listChannel.end(); it++)
		{
			if (this->_s->getChannel(it->first)->userIsInChann(fd) == true)
				return true;
			else
				return false;
		}
	}
	return false;
}

void	Commands::quitCmd()
{
	if(this->_line_cmd.size() == 2 && this->_line_cmd[1] != ":leaving")
	{
		if(userOnChannel() == true)
		{
			this->_msg->RPL_QUIT(this->_user->getNickname(), this->_user->getUser(), joinMessages(1) , rplToAllChan());
			if (this->_user->nicknameIsSet() == false)
				std::cout << " >> " << RED << "Client " << this->_fd_user << " has been disconnected." << Color << std::endl;
			else
				std::cout << " >> " << RED << this->_user->getNickname() << " has been disconnected." << Color << std::endl;
			this->_s->setFdUsersDc(this->_fd_user);
		}
	}
	else
	{
		if (this->_user->nicknameIsSet() == false)
			std::cout << " >> " << RED << "Client " << this->_fd_user << " has been disconnected." << Color << std::endl;
		else
			std::cout << " >> " << RED << this->_user->getNickname() << " has been disconnected." << Color << std::endl;
		this->_s->setFdUsersDc(this->_fd_user);
	}
}

