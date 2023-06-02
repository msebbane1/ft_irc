/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quitCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:58:17 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/02 13:00:16 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//---------------------QUIT-------------------//

//  [<Message de départ >]
void	Commands::quitCmd() // QUIT <msg>
{
	//if (this->_line_cmd.size > 2) 
		//afficher msg dans tous les channels + msg
	this->_s->setFdUsersDc(this->_fd_user);
	if(this->_user->nicknameIsSet() == false)
		std::cout << "Client " << this->_fd_user << " has been disconnected." << std::endl;
	else
		std::cout << this->_user->getNickname() << " has been disconnected." << std::endl;
}

