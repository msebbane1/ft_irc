/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeUserCmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 14:30:22 by clecat            #+#    #+#             */
/*   Updated: 2023/06/22 11:19:45 by msebbane         ###   ########.fr       */
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
			//case 'i': //+i : invisible, on ne te verra pas en faisant un /NAMES ou un /WHO sur un channel.
				//setChanInviteOnlyMode();
				//break;

			default:
				this->_msg->ERR_UMODUUNKNOWNFLAG(this->_line_cmd[1], this->_fd_user); //checker l'option(mode) i, t, k, o, l; 
				break;
		}
	}
}

// + user devient invisible; - user redevient visible
void	Commands::setInvisibleMode()
{
	if(this->getIndice() == '+')
		this->_s->getClient(this->_line_cmd[1])->setInvisible(true);
	else if (this->getIndice() == '-')
		this->_s->getClient(this->_line_cmd[1])->setInvisible(false);
}
