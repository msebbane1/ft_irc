/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 16:05:43 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/12 16:06:50 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client* c): _name(name), _creator(c), _key(),
											_topic(), _list_user_co(), _list_operators(),
											_list_banned(), _password(), _size_max(10),
											_i_only(false)
{
	this->addUser(c, c->get_fd());
	this->addOperator(c, c->get_fd());
}

Channel::Channel(std::string name,  Client* c, std::string key): _name(name), _creator(c), _key(key),
																_topic(), _list_user_co(), _list_operators(),
																_list_banned(), _password(), _size_max(10),
																_i_only(false)
{
	this->addUser(c, c->get_fd());
	this->addOperator(c, c->get_fd());
}

Channel::~Channel()
{
}

Channel*	Channel::operator=(Channel const *c)
{
	this->_name = c->_name;
	this->_creator = c->_creator;
	this->_key = c->_key;
	this->_topic = c->_topic;
	this->_list_user_co = c->_list_user_co;
	this->_list_operators = c->_list_operators;
	this->_list_banned = c->_list_banned;
	this->_password = c->_password;
	return (this);
}

void	Channel::banUser(std::string username)
{
	for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
	{
		if (it->second->getUser() == username || it->second->getNickname() == username)
		{
			this->_list_user_co.erase(it->first);
			this->_list_banned.push_back(it->second->getNickname());
			std::cout << "User " << it->second->getNickname() << " has been ban from " << this->_name << std::endl; 
			return ;
		}
	}
}

//ajouter 
void	Channel::unbanUser(std::string username)
{
	if(isBanned(username))
	{
		for (std::map<int, Client *>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
		{
			if (it->second->getUser() == username || it->second->getNickname() == username)
			{
				addUser(it->second, it->first);
				//this->_list_banned.push_back(it->second->getNickname()); //remove user from list
				std::cout << "User " << it->second->getNickname() << " has been unban from " << this->_name << std::endl; 
				return ;
			}
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

void	Channel::displayOp()
{
	for (std::map<int, Client *>::iterator it = this->_list_operators.begin(); it != this->_list_operators.end(); it++)
	{
		if (!it->second->getNickname().empty())
			std::cout << "- " << ((it->second->getNickname().empty()) ? it->second->getUser() : it->second->getNickname()) << std::endl;
	}
}

bool	Channel::userIsInChann(int user)
{
	for (std::map<int, Client*>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
		if (it->first == user)
			return (true);
	return (false);
}

void	Channel::sendMsg(int user_talk, std::string msg)
{
	for (std::map<int, Client*>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
		if (it->first != user_talk)
			send(it->first, msg.c_str(), msg.size(), 0);
}

bool	Channel::isOperator(int fd)
{
	for (std::map<int, Client *>::iterator itop = this->_list_operators.begin(); itop != this->_list_operators.end(); itop++)
	{
		if (itop->first == fd)
			return (true);
	}
	return (false);
}

int		Channel::nbUserInChan()
{
	int	i = 0;
	for (std::map<int, Client*>::iterator it = this->_list_user_co.begin(); it != this->_list_user_co.end(); it++)
		i++;
	return (i);
}

bool	Channel::isBanned(std::string nickname)
{
	if (!this->_list_banned.empty())
	{
		for (std::vector<std::string>::iterator it = this->_list_banned.begin(); it != this->_list_banned.end(); it++)
		{
			if (*it == nickname)
				return (true);
		}
	}
	return (false);
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

std::string	Channel::getTopic()
{
	return (this->_topic);
}

void		Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

bool		Channel::topicIsSet()
{
	if(this->_topic.empty())
		return false;
	else
		return true;
}

/*---------------------------------------------------------------------*/

std::string	Channel::getKey()
{
	return (this->_key);
}

void		Channel::setKey(std::string key)
{
	this->_key = key;
}

/*---------------------------------------------------------------------*/

int	Channel::getSizeMax()
{
	return (this->_size_max);
}

void	Channel::setSizeMax(int size)
{
	this->_size_max = size;
}

void	Channel::setInviteOnly(bool set){

	this->_i_only = set;
}