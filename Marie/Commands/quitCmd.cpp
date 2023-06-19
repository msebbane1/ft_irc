/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quitCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:58:17 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/19 13:25:00 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
/*
	 Parameters: [<reason>]
*/
//---------------------QUIT-------------------//

void	Commands::quitCmd()
{
	if(this->_line_cmd.size() == 2)
	{
		this->_msg->RPL_QUIT(this->_user->getNickname(), this->_user->getUser(), joinMessages(1) , rplToAllChan());
		return ;
	}
	this->_s->setFdUsersDc(this->_fd_user);
	if (this->_user->nicknameIsSet() == false)
		std::cout << " >> " << RED << "Client " << this->_fd_user << " has been disconnected." << Color << std::endl;
	else
		std::cout << " >> " << RED << this->_user->getNickname() << " has been disconnected." << Color << std::endl;
}

