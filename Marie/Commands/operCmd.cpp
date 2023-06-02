/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:11:23 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/02 14:30:28 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void	Commands::operCmd() //params : <utilisateur> <mot de passe>
{
	if (this->_line_cmd.size() != 3)
	{
		_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if (this->_line_cmd[1] == _user->getNickname())
	{
		/*
		if (this->_line_cmd[2] != a l'operator pass)
		{
			_msg->ERR_PASSWDMISMATCH(this->_fd_user);
			return;
		}
		*/
		//else
			_msg->RPL_YOUREOPER(_user->getNickname(), this->_fd_user);
		/* a rajouter pour mode les acces operateur au client
		if (mode == o)
			setmod
		*/
	}
	
}