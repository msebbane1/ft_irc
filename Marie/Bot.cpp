/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:50:54 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/17 12:10:17 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool	Client::containBanWord(std::string str)
{
	std::vector<std::string>	banWords;
	std::string					tmp;
	unsigned int							i = 0;

	banWords.push_back("macron");
	banWords.push_back("leo");
	banWords.push_back("caca");

	while (i < str.length())
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