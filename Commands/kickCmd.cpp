/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:02:25 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 12:20:37 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	/connect localhost 8080 oui
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	ERR_USERNOTINCHANNEL (441)
	
	Exemples : KICK #Finnish Matthew 
	Parameters: <channel> <user> *( "," <user> ) [<comment>]
*/

void	Commands::kickCmd()
{
	std::string reason;
	if (this->_line_cmd.size() < 3)
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&') && chanExist(this->_line_cmd[1]) == true)
	{
		if (this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user))
		{
			if (this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user))
			{
				std::cout << " >> " << GREEN << "Accepted : channel operator" << Color << std::endl;
				if (this->_s->clientExist(this->_line_cmd[2]) && this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_s->getClient(this->_line_cmd[2])->get_fd()) == true) // et qui est dans le chan
				{
					if(this->_line_cmd[2] == this->_s->getClient(this->_line_cmd[2])->getNickname()) //verifier a ce quíl ne se remove pas lui meme ?
					{
						if(joinMessages(3) == ": ")
							reason = ":no reason send";
						else
							reason = joinMessages(3);
						std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
						for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++)
						{
							this->_msg->RPL_KICK(_user->getNickname(), _user->getUser(), this->_line_cmd[1], this->_s->getClient(this->_line_cmd[2])->getNickname(), reason, it->first);
						}
						std::cout << " >> " << RED << this->_s->getClient(this->_line_cmd[2])->getNickname() << " is kicked from channel " << this->_line_cmd[1] << Color << std::endl;
						this->_s->getChannel(this->_line_cmd[1])->removeUser(this->_s->getClient(this->_line_cmd[2])->getNickname());
					}
				}
				else
				{
					this->_msg->ERR_USERNOTINCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
					return ;
				}
			}
			else
			{
				std::cout << " >> " << RED << "Denied : not channel operator" << Color << std::endl;
				this->_msg->ERR_CHANOPRIVSNEEDED(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
				return ;
			}
		}
		else
			this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
	}
	else
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
}