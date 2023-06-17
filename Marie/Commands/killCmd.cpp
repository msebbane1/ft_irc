/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   killCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:09:18 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/17 16:53:33 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	ERR_NOSUCHSERVER (402)
	ERR_NEEDMOREPARAMS (461)
	ERR_NOPRIVILEGES (481)
	ERR_NOPRIVS (723)
	
	params = <pseudonyme> <commentaire>
	
*/
///============KILL==========//

void	Commands::killCmd()
{
	if (this->_line_cmd.size() < 2)
	{
		_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if (this->_s->getClient(this->_line_cmd[1])->getIRCOperator() == false)
	{
		//error / 481
		std::cout << "not operator" << std::endl;
		return ;
	}
	else
	{
		this->_s->setFdUsersDc(this->_fd_user);
	}

}