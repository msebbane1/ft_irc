/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:11:23 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/12 17:37:05 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
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
		_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if (this->_line_cmd[1] == this->_user->getNickname())
	{
		if (this->_line_cmd[2] != this->_s->getPasswordOper())
		{
			_msg->ERR_PASSWDMISMATCH(this->_fd_user);
			return;
		}
		else
			_msg->RPL_YOUREOPER(_user->getNickname(), this->_fd_user);
		/* a rajouter pour mode les acces operateur au client
		if (getIrcOperator )
			setIrcOperator
		*/
	}
	
}