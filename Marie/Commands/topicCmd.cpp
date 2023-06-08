/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 11:02:11 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/08 12:19:27 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	RPL_NOTOPIC (331)
	RPL_TOPIC (332)
	RPL_TOPICWHOTIME (333)

	Exemples :
	TOPIC #test :New topic          ; Setting the topic on "#test" to "New topic"
  	TOPIC #test :                   ; Clearing the topic on "#test"
  	TOPIC #test                     ; Checking the topic for "#test"

	Parameters: <channel> [<topic>]
*/
void	Commands::topicCmd()
{
	return ;
}