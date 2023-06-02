/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   killCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:09:18 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/02 15:12:35 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

///============KILL==========//

void	Commands::killCmd()// <pseudonyme> <commentaire>
{
	if (this->_line_cmd.size() < 2)
	{
		_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	/*
	if(client est operator)
	{
		if()
	}
	*/
}