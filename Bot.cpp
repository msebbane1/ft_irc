/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:50:54 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/21 14:44:55 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool	Channel::containBanWord(std::string str)
{
	std::vector<std::string>	banWords;
	std::string					tmp;
	size_t							i = 0;

	banWords.push_back("macron");
	banWords.push_back("leo");
	banWords.push_back("caca");

	while (i < str.size())
	{
		str[i] = std::tolower(str[i]);
		i++;
	}
	i = 0;
	while (i < banWords.size())
	{
		if (str.find(banWords[i]) != std::string::npos)
			return (true);
		i++;
	}
	return (false);
}