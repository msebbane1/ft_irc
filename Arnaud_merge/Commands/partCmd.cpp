/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:03:01 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 12:51:15 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// /connect localhost 8080 oui
#include "Commands.hpp"

/*
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)

	Exemples :
	PART #twilight_zone             ; leave channel "#twilight_zone"
  	PART #oz-ops,&group5            ; leave both channels "&group5" and "#oz-ops" // FAIT

	Parameters: <channel>{,<channel>} [<reason>]
*/

//==============PART=============//


void	Commands::leaveMultiChan()
{
	std::vector<std::string> channels = _s->splitCustom(this->_line_cmd[1], ',');
	
	for(unsigned int i = 0; i < channels.size(); i++)
	{
		if ((channels[i][0] == '#' || channels[i][0] == '&') && chanExist(channels[i]) == true)
		{
			std::map<int, Client*> UserCo = this->_s->getChannel(channels[i])->getListUserCo();
			if(this->_s->getChannel(channels[i])->userIsInChann(this->_fd_user) == true)
			{
				for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
				{
					this->_msg->RPL_LEFTCHANNEL(this->_user->getNickname(), this->_user->getUser(), channels[i], it->first);
				}
				this->_s->getChannel(channels[i])->removeUser(_user->getNickname());
				std::cout << " >> " << RED << this->_user->getNickname() << " has left channel " << channels[i] << Color << std::endl;
			}
			else
				this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), channels[i], this->_fd_user);
		}
		else
		{
			this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
			return ;
		}
	}
}

void	Commands::partCmd()
{
	std::string::iterator searchMulti = find(this->_line_cmd[1].begin(), this->_line_cmd[1].end(), ',');
	if (this->_line_cmd.size() < 1)
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return ;
	}
	if(*searchMulti == ',')
	{
		leaveMultiChan();
		return ;
	}
	if ((this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&') && chanExist(this->_line_cmd[1]) == true && *searchMulti != ',') // a verifier sur nc
	{
		std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
		if(this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true)
		{
			for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
			{
				this->_msg->RPL_LEFTCHANNEL(this->_user->getNickname(), this->_user->getUser(), this->_line_cmd[1], it->first);
			}
			this->_s->getChannel(this->_line_cmd[1])->removeUser(this->_user->getNickname());
			std::cout << " >> " << RED << this->_user->getNickname() << " has left channel " << this->_line_cmd[1] << Color << std::endl;
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