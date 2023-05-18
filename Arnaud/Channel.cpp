/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 16:05:43 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/18 17:14:17 by asahonet         ###   ########.fr       */
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
	for (int i = 0; i < this->_list_banned.size(); i++)
	{
		if (this->_list_banned[i] == fd_cl)
		{
			// envoyer un message comme quoi il est ban
			// il faut d'abord le deban pur l'add
			return ;
		}
	}
	this->_list_user_co.insert(std::pair<int, Client*>(fd_cl, cl));
}

void	Channel::removeUser(std::string username)
{
	Client *c;
	
	for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
	{
		c = it->second();
		if (c->getUser() == username || c->getNickname() == username)
		{
			this->_list_user_co.erase(it->first());
			return ;
		}
	}
}

void	Channel::banUser(std::string username)
{
	Client *c;

	for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
	{
		c = it->second;
		if (c->getUser() == username || c->getNickname() == username)
		{
			this->_list_user_co.erase(it->first);
			this->_list_banned.insert(it->first);
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

std::vector<std::string>	Channel::getHisto()
{
	return (this->_historic);
}

void						Channel::addHisto(std::string msg)
{
	this->_historic.insert(msg);
}