/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 16:05:43 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/22 13:50:35 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name): _name(name)
{
}

Channel::Channel(std::string name, std::string pw): _name(name), _password(pw)
{
}

Channel::~Channel()
{
}

void	Channel::addUser(Client *cl, int fd_cl)
{
	for (unsigned long i = 0; i < this->_list_banned.size(); i++)
	{
		if (this->_list_banned[i] == fd_cl)
		{
			std::cout << "User has been banned. Can't join the channel." << std::endl;
			// envoyer un message comme quoi il est ban
			// il faut d'abord le deban pur l'add
			return ;
		}
	}
	this->_list_user_co.insert(std::pair<int, Client*>(fd_cl, cl));
}

void	Channel::removeUser(std::string username)
{
	for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
	{
		if (it->second->getUser() == username || it->second->getNickname() == username)
		{
			this->_list_user_co.erase(it->first);
			return ;
		}
	}
}

void	Channel::banUser(std::string username)
{
	for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
	{
		if (it->second->getUser() == username || it->second->getNickname() == username)
		{
			this->_list_user_co.erase(it->first);
			this->_list_banned.push_back(it->first);
			return ;
		}
	}
}

void	Channel::displayUsers()
{
	for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
	{
		if (!it->second->getNickname().empty())
		std::cout << "- " << ((it->second->getNickname().empty()) ? it->second->getUser() : it->second->getNickname()) << std::endl;
	}
}

/*std::vector<std::string>	Channel::getHisto()
{
	return (this->_historic);
}

void						Channel::addHisto(std::string msg)
{
	this->_historic.insert(msg);
}*/

std::string					Channel::getName()
{
	return (this->_name);
}

void						Channel::setName(std::string name)
{
	this->_name = name;
}

std::map<int, Client*>		Channel::getListUserCo()
{
	return (this->_list_user_co);
}