/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:11:23 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 08:20:49 by msebbane         ###   ########.fr       */
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
	if (this->_line_cmd[1] == this->_user->getNickname() && this->_s->getClient(this->_line_cmd[1])->getIRCOperator() == false)
	{
		if (this->_line_cmd[2] != this->_s->getPasswordOper())
		{
			std::cout << "line = " << this->_line_cmd[2] << " et password = " << this->_s->getPasswordOper() <<std::endl;
			this->_msg->ERR_PASSWDMISMATCH(this->_fd_user);
			std::cout << " >> " << RED << " Password incorrect for Operator.. " << Color << std::endl;
			return;
		}
		else
		{
			if(this->_s->getClient(this->_line_cmd[1])->getIRCOperator() == false)
				this->_s->getClient(this->_line_cmd[1])->setIRCOperator(true);
			this->_msg->RPL_YOUREOPER(_user->getNickname(), this->_fd_user);
			std::cout << " >> " << GREEN << _user->getNickname() << " is Operator " << Color << std::endl;
		}
	}
	else
	{
		std::cout << " >> " << YELLOW << _user->getNickname() << " is already Operator " << Color << std::endl;
		return ;
	}
		
}