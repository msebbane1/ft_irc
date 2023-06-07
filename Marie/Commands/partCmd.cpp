/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:03:01 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/07 14:39:03 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 // /connect localhost 8080 oui
#include "Commands.hpp"


/*
           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL
		   
*/
//==============PART=============//

void	Commands::partCmd() //  <canal>{,< canal >}
{
	std::cout << "passs" << std::endl;
	if (this->_line_cmd.size() < 1)
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return ;
	}
	if (this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&')
	{
		std::cout << "hello" << std::endl;
		std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
		if (chanExist(this->_line_cmd[1]) == true)
		{
			std::cout << "chanexist" << std::endl;
			if(this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true)
			{
				for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
				{
				    this->_msg->RPL_LEFTCHANNEL(_user->getNickname(), this->_user->getUser(), this->_line_cmd[1], it->first);
				}
			}
			else
				this->_msg->ERR_NOTONCHANNEL(this->_line_cmd[1], _fd_user);
			this->_s->getChannel(this->_line_cmd[1])->removeUser(_user->getNickname());
		}
	}
	else
	{
		std::cout << "hello2" << std::endl;
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], _fd_user);
		return ;
	}
	if (this->_s->getChannel(this->_line_cmd[1])->getListUserCo().empty())
	{
		std::cout << "holla" << std::endl;
       this->_s->channDisconnected();
	}
	return ;
}