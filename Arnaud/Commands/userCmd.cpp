/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:58:20 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/05 10:50:56 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//---------------------USER-------------------//

// Paramètres: <nom d'utilisateur> <hôte> <nom de serveur> <nom réel>
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
			std::string	realname = this->_line_cmd[4] + (this->_line_cmd[5].empty() ? "\0" : " " + this->_line_cmd[5]);
			this->_user->setRealname(realname);
			this->_user->setUser(this->_line_cmd[1]);
			std::cout << "====== Realname is set ===== :" << this->_user->getRealname() << "\n";
			std::cout << "====== USER is set ===== :" << this->_user->getUser() << "\n";
		}
	}
	else if(_line_cmd[0] == "USER" && this->_user->realnameIsSet())
		this->_msg->ERR_ALREADYREGISTRED(this->_fd_user);
		
}