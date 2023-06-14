/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quitCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:58:17 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/13 14:46:53 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
/*
	 Parameters: [<reason>]
*/
//---------------------QUIT-------------------//

void	Commands::quitCmd()
{
	if (this->_line_cmd.size() > 0) // a envoyer dans le chan ?
		_msg->RPL_QUIT(_user->getNickname(), _user->getUser(), joinMessages() , this->_fd_user);
	this->_s->setFdUsersDc(this->_fd_user);
	if (this->_user->nicknameIsSet() == false)
		std::cout << "Client " << this->_fd_user << " has been disconnected." << std::endl;
	else
		std::cout << this->_user->getNickname() << " has been disconnected." << std::endl;
}

