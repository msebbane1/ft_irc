/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:58:20 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 08:35:52 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	ERR_NEEDMOREPARAMS (461)
	ERR_ALREADYREGISTERED (462)
	
	Paramètres: <nom d'utilisateur> <hôte> <nom de serveur> <nom réel>
*/

//---------------------USER-------------------//

void	Commands::userCmd()
{
	std::string realname;
	
	if(!this->_user->realnameIsSet())
	{
		if (this->_line_cmd.size() < 5 || this->_line_cmd[4][0] != ':')
		{
			this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
			return;
		}
		if (this->_line_cmd[4][0] == ':' && this->_line_cmd[4].size() >= 2)
		{
			this->_line_cmd[4] = this->_line_cmd[4].substr(1, this->_line_cmd[4].size() - 1);
			if(this->_line_cmd.size() == 5)
			{
				std::string realname = this->_line_cmd[4];
				this->_user->setRealname(realname);
			}
			else if(this->_line_cmd.size() >= 2 && this->_line_cmd.size() != 5)
			{
				std::string realname = this->_line_cmd[4] + " " + this->_line_cmd[5];
				this->_user->setRealname(realname);
			}
			this->_user->setUser(this->_line_cmd[1]);
			std::cout << " >> " << GREEN << "Realname: " << this->_user->getRealname() << Color <<"\n";
			std::cout << " >> " << GREEN << "User: " << this->_user->getUser() << Color <<"\n";
		}
	}
	else if(_line_cmd[0] == "USER" && this->_user->realnameIsSet())
		this->_msg->ERR_ALREADYREGISTRED(this->_fd_user);
		
}