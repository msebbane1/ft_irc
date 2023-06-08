/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:03:01 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/08 12:07:38 by msebbane         ###   ########.fr       */
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
	if (this->_line_cmd.size() < 1)
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return ;
	}
	if ((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&') && chanExist(this->_line_cmd[1]) == true)
	{
		std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
		if(this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true)
		{
			for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
			{
				this->_msg->RPL_LEFTCHANNEL(this->_user->getNickname(), this->_user->getUser(), this->_line_cmd[1], it->first);
			}
			this->_s->getChannel(this->_line_cmd[1])->removeUser(_user->getNickname());
		}
		else
			this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user); // verif sur vrai server le msg exact
	}
	else
	{
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
		return ;
	}
	if (this->_s->getChannel(this->_line_cmd[1])->getListUserCo().empty())
       this->_s->channDisconnected();
}