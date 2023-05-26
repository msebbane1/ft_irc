/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 16:05:43 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/26 10:26:29 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client* c): _name(name), _creator(c), _key(),
											_topic(), _list_user_co(), _list_operators(),
											_list_banned(), _password()
{
}

Channel::Channel(std::string name,  Client* c, std::string key): _name(name), _creator(c), _key(key),
																_topic(), _list_user_co(), _list_operators(),
																_list_banned(), _password()
{
}

Channel::~Channel()
{
}

void	Channel::banUser(std::string username)
{
	for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
	{
		if (it->second->getUser() == username || it->second->getNickname() == username)
		{
			this->_list_user_co.erase(it->first);
			this->_list_banned.push_back(it->second->getUser());
			std::cout << "User " << it->second->getNickname() << " has been ban from " << this->_name << std::endl; 
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

/*---------------------------------------------------------------------*/

std::string	Channel::getName()
{
	return (this->_name);
}

void		Channel::setName(std::string name)
{
	this->_name = name;
}

/*---------------------------------------------------------------------*/

std::map<int, Client*>	Channel::getListUserCo()
{
	return (this->_list_user_co);
}

void					Channel::addUser(Client *cl, int fd_cl)
{
	for (unsigned long i = 0; i < this->_list_banned.size(); i++)
	{
		if (this->_list_banned[i] == cl->getUser())
		{
			std::cout << "User has been banned. Can't join " << this->_name << std::endl;
			// envoyer un message comme quoi il est ban
			// il faut d'abord le deban pur l'add
			return ;
		}
	}
	this->_list_user_co.insert(std::pair<int, Client*>(fd_cl, cl));
	// user added to chann
}

void					Channel::removeUser(std::string username)
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

/*---------------------------------------------------------------------*/

std::map<int, Client*>	Channel::getListOp()
{
	return (this->_list_operators);
}

void					Channel::addOperator(Client *cl, int fd_cl)
{
	for (unsigned long i = 0; i < this->_list_banned.size(); i++)
	{
		if (this->_list_banned[i] == cl->getUser())
		{
			std::cout << "User has been banned. Can't join " << this->_name << std::endl;
			// envoyer un message comme quoi il est ban
			// il faut d'abord le deban pur l'add
			return ;
		}
	}
	this->_list_operators.insert(std::pair<int, Client*>(fd_cl, cl));
	// user added like operator
}

void					Channel::removeOperator(std::string username)
{
	for (std::map<int, Client *>::iterator it = this->_list_operators.begin(); it != this->_list_operators.end(); it++)
	{
		if (it->second->getUser() == username || it->second->getNickname() == username)
		{
			this->_list_operators.erase(it->first);
			return ;
		}
	}
}

/*---------------------------------------------------------------------*/
