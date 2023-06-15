/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:11:23 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/15 13:09:50 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	/connect localhost 8080 oui
	ERR_NEEDMOREPARAMS (461)
	ERR_PASSWDMISMATCH (464)
	ERR_NOOPERHOST (491)
	RPL_YOUREOPER (381)

	Exemple :
	OPER foo bar                ; Attempt to register as an operator
                              using a name of "foo" and the password "bar".
	Parameters: <name> <password>
							  
*/
void	Commands::operCmd()
{
	if (this->_line_cmd.size() != 3)
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if (this->_line_cmd[1] == this->_user->getNickname())
	{
		if (this->_line_cmd[2] != this->_s->getPasswordOper())
		{
			this->_msg->ERR_PASSWDMISMATCH(this->_fd_user);
			return;
		}
		else
		{
			if(this->_s->getClient(this->_line_cmd[1])->getIRCOperator() == false)
				this->_s->getClient(this->_line_cmd[1])->setIRCOperator(true);
			this->_msg->RPL_YOUREOPER(_user->getNickname(), this->_fd_user);
		}
	}
	
}