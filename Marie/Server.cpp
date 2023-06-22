/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:41:57 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/22 18:16:20 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _passwordOper("password")
{
}

Server::~Server()
{
	std::cout << RED << "Server shutting down..." << Color << std::endl;
	return ;
}

/*--------------------------------------------------------*/

void					Server::setFdUsersDc(int fdUsersDc)
{
	this->_fd_users_dc.push_back(fdUsersDc);
}

std::vector<int>		Server::getFdUsersDc()
{
	return (this->_fd_users_dc);
}

/*--------------------------------------------------------*/
std::string				Server::getPassword()
{
    return (this->_password);
}

std::string				Server::getPasswordOper()
{
    return (this->_passwordOper);
}

void					Server::setPassword(std::string pwd)
{
    this->_password = pwd;
}

/*--------------------------------------------------------*/

std::map<int, Client*>	Server::getListClient()
{
	return (this->_list_client);
}

Client *Server::getClient(std::string nick) 
{	for (std::map<int, Client *>::iterator it = _list_client.begin(); it != _list_client.end(); it++) 
	{
		if (it->second->getNickname() == nick)
			return (it->second);
	}
	return (NULL);
}


void		Server::setListClient(int fd, Client *user)
{
	this->_list_client.insert(std::pair<int, Client*>(fd, user));
}

/*--------------------------------------------------------*/
std::map<std::string, Channel*>	Server::getListChan()
{
	return (this->_list_chan);
}

Channel*						Server::getChannel(std::string chann)
{
	return (this->_list_chan[chann]);
}

void							Server::addListChan(Channel *c)
{
	this->_list_chan.insert(std::pair<std::string, Channel*>(c->getName(), c));
}

/*--------------------------------------------------------*/

bool Server::clientExist(std::string nick) 
{
	for (std::map<int, Client *>::iterator it = _list_client.begin(); it != _list_client.end(); it++) 
	{
		if (it->second->getNickname() == nick)
			return (true);
	}
	return (false);
}

/*--------------------------------------------------------*/

void		Server::createServ(int port)
{
	Messages	msg;
	int			opt = 1;
	
	if ((this->_fd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		msg.errorMsg("socket failed");
	
	if (setsockopt(this->_fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		msg.errorMsg("setsockopt");
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	
	if (bind(this->_fd_server, (struct sockaddr*) &this->_addr, sizeof(this->_addr)) < 0)
		msg.errorMsg("bind failed");
	this->_addr_len = sizeof(this->_addr);
	
	if (listen(this->_fd_server, 500) < 0)
		msg.errorMsg("listen");
	
	this->_command_list.push_back("PASS");
	this->_command_list.push_back("NICK");
	this->_command_list.push_back("USER");
	this->_command_list.push_back("PRIVMSG");
	this->_command_list.push_back("JOIN");
	this->_command_list.push_back("QUIT");
	this->_command_list.push_back("PING");
	this->_command_list.push_back("CAP");
	this->_command_list.push_back("WHOIS");
	this->_command_list.push_back("KICK");
	this->_command_list.push_back("INVITE");
	this->_command_list.push_back("TOPIC");
	this->_command_list.push_back("MODE");
	this->_command_list.push_back("NAMES");
	this->_command_list.push_back("PART");
	this->_command_list.push_back("OPER");
	this->_command_list.push_back("WHO");
	this->_command_list.push_back("KILL");
	this->_command_list.push_back("kill");
	
	std::cout << Blue << "Listen to port : " << port << Color << std::endl;
}

/*--------------------------------------------------------*/

bool	Server::isCommandIrc(std::string str)
{

	for (unsigned int i = 0; i < this->_command_list.size(); i++)
	{
		if (this->_command_list[i] + '\n' == str || this->_command_list[i] == str)
			return (true);
	}
	return (false);
}

/*--------------------------------------------------------*/

int		Server::countCharInString(std::string buf, char c)
{
	int	count = 0;

	for (unsigned long i = 0; (i = buf.find(c, i)) != std::string::npos; i++)
        count++;
	return (count);
}

std::vector<std::string>	Server::splitCustom(std::string buf, char charset)
{
	std::string					tmp;
	std::vector<std::string>	split;
	unsigned long				e_i = 0;
	unsigned long				s_i = 0;

	for (unsigned long i = 0; i <= buf.size(); i++)
	{
		if (buf[i] == charset || i == buf.size())
		{
			e_i = i;
			tmp.clear();
			tmp.append(buf, s_i, e_i - s_i);
			if (!tmp.empty())
				split.push_back(tmp);
			s_i = e_i + 1;
		}
	}
	return (split);
}

/*--------------------------------------------------------*/

void	Server::connectToClients(int user_talk, std::string buf)
{
	Messages msg;
	if (buf.find("\r\n") != std::string::npos)
		buf = buf.substr(0, buf.length() - 1);
	if (buf.length() > 1) 
		buf = buf.substr(0, buf.length() - 1);
	std::vector<std::string>	line = splitCustom(buf, ' ');
	
	Commands *cmd = new Commands(this, _list_client[user_talk], user_talk, line, msg);
	cmd->exec_cmd();
	delete cmd;
}

/*--------------------------------------------------------*/

void Server::connect(int user_talk, std::string buf)
{
	std::string msg;
	
	if (buf.length() > 1000)
	{
		msg = "Error: limit char";
		send(user_talk, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (buf[0] == '\n' || buf[0] == '\t')
		return ;
	connectToClients(user_talk, buf);
}

/*--------------------------------------------------------*/

void	Server::acceptUser()
{
	Messages msg;
	int	max_fd;
	int	new_user;
	int	fd_user;
	
	FD_ZERO(&this->_fds);
	FD_SET(this->_fd_server, &this->_fds);
	
	max_fd = this->_fd_server;
	for (std::map<int, Client *>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		fd_user = it->first;
		FD_SET(fd_user, &this->_fds);
		fcntl(fd_user, F_SETFL, O_NONBLOCK);
		max_fd = std::max(max_fd, fd_user);
	}
	if (select(max_fd + 1, &this->_fds, NULL, NULL, NULL) < 0)
		msg.errorMsg("select");
	if (FD_ISSET(this->_fd_server, &this->_fds))
	{
		if ((new_user = accept(this->_fd_server, (struct sockaddr*) &this->_addr,(socklen_t*) &this->_addr_len)) < 0)
			msg.errorMsg("accept");
		Client *cl = new Client();
		this->_list_client.insert(std::pair<int, Client*>(new_user, cl));
		this->_list_client[new_user]->set_fd(new_user);
		std::cout << std::endl;
		std::cout << " >> " << GREEN << " [~New client connected~] [ID: "<< new_user << "]" << Color << std::endl;
	}
}

/*--------------------------------------------------------*/

int		Server::received(char *buffer, int user_talk)
{
	int		n = 0;
	int		bytes_recv = 0;
	bool	bl_n = false;
				
	while (bl_n == false && (n = recv(user_talk, buffer + bytes_recv, 1024 - bytes_recv - 1, 0)) > 0)
	{
		bytes_recv += n;
		if (buffer[bytes_recv - 1] == '\n')
			bl_n = true;
	}
	buffer[bytes_recv] = 0;
	return (bytes_recv);
}

/*--------------------------------------------------------*/

void	Server::clientDisconnected() 
{
	
	for (std::vector<int>::iterator it = this->_fd_users_dc.begin(); it != this->_fd_users_dc.end(); it++) {
		close(this->_list_client.find(*it)->first);
		delete this->_list_client.find(*it)->second;
		this->_list_client.erase(this->_list_client.find(*it)->first);
	}
	this->_fd_users_dc.clear();
}



void	Server::channDisconnected()
{
	std::map<std::string, Channel*> map = this->_list_chan;
	std::map<std::string, Channel*>::iterator it = map.begin();
	
	while (it != map.end())
	{
		if (it->second->getListUserCo().size() == 1)
		{
			this->_list_chan.erase(it->first);
			delete it->second;
		}
		it++;
	}
}

/*--------------------------------------------------------*/

void	Server::serverIrc()
{
	while (true)
	{
		Messages msg;
		char	buffer[1024] = { 0 };
		int		user_talk;

		acceptUser();
		
		for (std::map<int, Client *>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
		{
			user_talk = it->first;
			if (FD_ISSET(user_talk, &this->_fds))
			{
				int		bytes_recv = 0;
				
				bytes_recv = received(buffer, user_talk);
				if(bytes_recv <= 0)
				{
					this->_fd_users_dc.push_back(user_talk);
					std::cout << ">>" << RED << "Client " << user_talk << " has been disconnected." << Color << std::endl;
				}
				if (strncmp(buffer, "", 1) != 0)
				{
					int	nb;
					msg.displayMsgOnServer(buffer, user_talk);
					nb = countCharInString(buffer, '\n');
					if (nb > 1)
					{
						std::vector<std::string>	line;
						int							i = 0;
						line = splitCustom(buffer, '\n');
						while (i < nb)
						{
							msg.displayMsgOnServer(line[i], user_talk);
							connect(user_talk, line[i]);
							i++;
						}
					}
					else
						connect(user_talk, buffer);
				}
			}
		}
		if (!this->_fd_users_dc.empty())
			clientDisconnected();
		if (!this->_list_chan.empty())
			channDisconnected();
	}
}