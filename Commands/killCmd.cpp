/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   killCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:09:18 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 12:04:21 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	/connect localhost 8080 oui
	ERR_NEEDMOREPARAMS (461)
	ERR_NOPRIVILEGES (481)
	
	ERR_NOSUCHSERVER (402)
	ERR_NOPRIVS (723)
	
	params = <pseudonyme> <commentaire>
	
*/
///============KILL==========//

Channel*	Commands::rplToAllChan()
{
	Channel *c;
	int fd;
	std::map<std::string, Channel *> listChannel = _s->getListChan();
	
	std::map<int, Client*>	listClient = _s->getListClient();
	
	for (std::map<int, Client*>::iterator it = listClient.begin(); it != listClient.end(); it++)
	{
		fd = it->first;
		for (std::map<std::string, Channel *>::iterator it = listChannel.begin(); it != listChannel.end(); it++)
		{
			if (this->_s->getChannel(it->first)->userIsInChann(fd) == true)
				c = this->_s->getChannel(it->first);
		}
	}
	return(c);
}

void	Commands::killCmd()
{
	std::string reason;
	if (this->_line_cmd[1].empty())
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if (this->_s->getClient(this->_user->getNickname())->getIRCOperator() == false)
	{
		this->_msg->ERR_NOPRIVILEGES(this->_user->getNickname(), this->_fd_user);
		std::cout << " >> " << RED << "Denied : not server operator" << Color << std::endl;
		return ;
	}
	if(this->_s->clientExist(this->_line_cmd[1]))
	{
		if(joinMessages(2) == ": ")
			reason = "No reason send";
		else
			reason = joinMessages(2);
		this->_msg->RPL_KILL(this->_user->getNickname(), this->_user->getUser(), this->_line_cmd[1], reason, this->_s->getClient(this->_line_cmd[1])->get_fd());
		std::string quit = ":Killed by " + this->_user->getNickname() + " :" + reason;
		this->_msg->RPL_QUIT(this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_s->getClient(this->_line_cmd[1])->getUser(), quit, rplToAllChan());
		std::string err = ":Closing Link: localhost. Killed by " + this->_user->getNickname() + " :" + reason;
		this->_msg->RPL_ERROR(this->_s->getClient(this->_line_cmd[1])->getNickname(), this->_s->getClient(this->_line_cmd[1])->getUser(), err, this->_s->getClient(this->_line_cmd[1])->get_fd());
		
		this->_s->setFdUsersDc(this->_s->getClient(this->_line_cmd[1])->get_fd());
		std::cout << " >> " << RED << this->_line_cmd[1] << " has been disconnected." << Color << std::endl;
		return ;

	}
	else
	{
		this->_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
		return;
	}

}