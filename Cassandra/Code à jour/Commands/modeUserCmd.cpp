/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeUserCmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 14:30:22 by clecat            #+#    #+#             */
/*   Updated: 2023/06/08 15:40:35 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//lien doc: https://datatracker.ietf.org/doc/html/rfc1459
#include "Commands.hpp"

//gestion Mode sur User

void	Commands::modeOnUser(){

	std::vector<char>::iterator	it = this->_optionList.begin();
	for(; it != this->_optionList.end(); it++)
	{
		char option = this->_optionList[*it];
		switch (option)
		{
			case 'i': //+i : invisible, on ne te verra pas en faisant un /NAMES ou un /WHO sur un channel.
				setChanInviteOnlyMode();
				break;

			case 'o' : //+o : pour avoir le status IRCOperator (IRCOperator)
				setChanOperator();
				break;

			default:
				this->_msg->ERR_UMODUUNKNOWNFLAG(this->_line_cmd[1], this->_fd_user); //checker l'option(mode) i, t, k, o, l; 
				break;
		}
	}
}

void	Commands::setInvisibleMode()
{
	// + user devient invisible
	// - user redevient visible
}

void	Commands::setUserOperator()
{
	// +o user devient operator
	// -o user n'est plus operator
}