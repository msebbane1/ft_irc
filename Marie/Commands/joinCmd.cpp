/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:56:31 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/02 13:00:43 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//---------------------JOIN-------------------//

// <canal>{,<canal>} [<clé>{,<clé>}]
void	Commands::joinCmd()
{
	std::string	msg;

	if (this->_line_cmd.size() == 1)
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user); //461
	else if (this->_line_cmd[1][0] != '#' || this->_line_cmd[1][0] != '&')
	{
		// A changer type erreur
		this->_msg->errorSend("", this->_user->getNickname(), "Wrong format", this->_user->get_fd());
		return;
	}
	if (chanExist(this->_line_cmd[1]) == false)
	{
		Channel	*chan = new Channel(this->_line_cmd[1], this->_user);
		this->_s->addListChan(chan);
	}
	else
	{
		this->_s->getChannel(this->_line_cmd[1])->addUser(this->_user, this->_fd_user);
	}
	msg = "You have joined successfully " + this->_line_cmd[1] + " !\n";
	send(this->_user->get_fd(), msg.c_str(), msg.size(), 0);
	
	// affichage tt user dans le chann
	std::cout << "----------------" << std::endl;
	this->_s->getChannel(this->_line_cmd[1])->displayUsers();
	std::cout << "----------------" << std::endl;
	this->_s->getChannel(this->_line_cmd[1])->displayOp();
}
