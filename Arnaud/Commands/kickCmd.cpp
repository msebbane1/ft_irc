/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:02:25 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/15 12:08:27 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	ERR_USERNOTINCHANNEL (441)
	
	Exemples : KICK #Finnish Matthew 
	Parameters: <channel> <user> *( "," <user> ) [<comment>]
*/

std::string	Commands::joinReason()
{
	std::string msg;
	std::vector<std::string>::iterator it = this->_line_cmd.begin() + 3;
	while (it != this->_line_cmd.end())
	{
		msg += *it;
		msg += " ";
		it++;
	}
	return(msg);
}

void	Commands::kickCmd()
{
	if (this->_line_cmd.size() < 3)
	{
		_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&') && chanExist(this->_line_cmd[1]) == true)
	{
		if (_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user))
		{
			if (_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user))
			{
				std::cout << "operator ok" << std::endl;
				if (_s->clientExist(this->_line_cmd[2]) && _s->getChannel(this->_line_cmd[1])->userIsInChann(this->_s->getClient(this->_line_cmd[2])->get_fd()) == true && this->_fd_user != _user->get_fd()) // et qui est dans le chan
				{
					std::cout << "operator ok22" << std::endl;
					if(this->_line_cmd[2] == this->_s->getClient(this->_line_cmd[2])->getNickname()) //verifier a ce quíl ne se remove pas lui meme
					{
						std::cout << "operator ok33" << std::endl;
						std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
						for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++)
							_msg->RPL_KICK(_user->getNickname(), _user->getUser(), this->_line_cmd[1], this->_s->getClient(this->_line_cmd[2])->getNickname(), joinReason(), it->first);
						this->_s->getChannel(this->_line_cmd[1])->removeUser(this->_s->getClient(this->_line_cmd[2])->getNickname());
					}
				}
				else
				{
					_msg->ERR_USERNOTINCHANNEL(this->_user->getNickname(), this->_line_cmd[1], _fd_user);
					return ;
				}
			}
			else
			{
				std::cout << "operator NOT" << std::endl;
				this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
				return ;
			}
		}
		else
			_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
	}
	else
		_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], _fd_user);
	if (this->_s->getChannel(this->_line_cmd[1])->getListUserCo().empty())
       this->_s->channDisconnected();
}