/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:41:57 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/16 11:47:08 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

/*--------------------------------------------------------*/

std::string	Server::getPassword()
{
    return (this->_password);
}

void		Server::setPassword(std::string pwd)
{
    this->_password = pwd;
}

/*--------------------------------------------------------*/

void		Server::createServ(int port)
{
	int	opt = 1;
	
	if ((this->_fd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		errorMsg("socket failed");
	
	if (setsockopt(this->_fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		errorMsg("setsockopt");
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	
	if (bind(this->_fd_server, (struct sockaddr*) &this->_addr, sizeof(this->_addr)) < 0)
		errorMsg("bind failed");
	this->_addr_len = sizeof(this->_addr);
	
	if (listen(this->_fd_server, 500) < 0)
		errorMsg("listen");
	std::cout << Blue << "Listen to port : " << port << Color << std::endl;
}

void		Server::errorMsg(std::string msg)
{
    std::cout << Red << msg << Color << std::endl;
	exit(EXIT_FAILURE);
}

void		Server::displayMsgOnServer(std::string const &buf, int user_talk)
{
	if(buf == "\n")
		return;
    std::cout << "| USER : client " << user_talk << " |" << std::endl;
    std::cout << "Message send :" << buf ;
}

/*--------------------------------------------------------*/

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
			split.push_back(tmp);
			s_i = e_i + 1;
		}
	}
	return (split);
}

bool	Server::connectToNc(std::string buf, int cl)
{
	std::vector<std::string>	line;

	line = splitCustom(buf, ' ');
	if (line[0] == "PASS")
	{
		if (line[1].find("\n") > 0)
			line[1].erase(line[1].size() - 1);
		if (line[1] == this->_password)
		{
			this->_list_client[cl]->setConnected();
			if (this->_historic.size() > 1)
				sendHistoric(cl);
			return (true);
		}
		this->_list_client[cl]->increment_pass_try();
		if (this->_list_client[cl]->get_pass_try() == 3)
		{
			close(cl);
			delete this->_list_client[cl]->get_user();
			this->_list_client.erase(cl);
		}
		else
		{
			std::string	msg;
			
			msg = "It left you " + std::to_string(3 - this->_list_client[cl]->get_pass_try()) + " try.\n";
			send(cl, msg.c_str(), msg.size(), 0);
		}
	}
	return (false);
}

/*--------------------------------------------------------*/

void	Server::userSendMsg(std::string const &buf, int user_talk)
{
	std::string	msg;
	int			fd_received;
	
	msg = "Message of [ID: " + std::to_string(user_talk) + "] : " + buf;
	if (buf[0] == '\n' || buf[0] == '\t')
		return ;
	if (this->_list_client[user_talk]->getConnected() == false && connectToNc(buf, user_talk) == false)
	{
		if (buf.find("PASS") == std::string::npos)
			msg = "You need to enter the password first\n";
		else
			msg = "You entered a wrong password.\n";
		send(user_talk, msg.c_str(), msg.size(), 0);
		return ;
	}
	if (this->_list_client[user_talk]->getConnected() == true)
	{
		for(std::map<int, Client *>::iterator ite = this->_list_client.begin(); ite != this->_list_client.end(); ite++)
		{
			fd_received = ite->first;
			if (fd_received != user_talk && this->_list_client[fd_received]->getConnected() == true)
			{
				if (send(fd_received, msg.c_str(), msg.length(), 0) < 0)
				{
					std::cout << "Send failed" << std::endl;
					close(user_talk);
					delete this->_list_client[user_talk]->get_user();
					this->_list_client.erase(user_talk);
					std::cout << "Client " << user_talk << " has been disconnected." << std::endl;
				}
			}
		}
		if (this->_list_client[user_talk])
			this->_historic.push_back(msg);
	}
}

void	Server::sendHistoric(int client_fd)
{
	for (unsigned int i = 0; i < this->_historic.size(); i++)
		send(client_fd, this->_historic[i].c_str() , this->_historic[i].size(), 0);
}

/*--------------------------------------------------------*/

void	Server::acceptUser()
{
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
		fcntl(fd_user, O_NONBLOCK);
		max_fd = std::max(max_fd, fd_user);
	}
	
	if (select(max_fd + 1, &this->_fds, NULL, NULL, NULL) < 0)
		errorMsg("select");
	if (FD_ISSET(this->_fd_server, &this->_fds))
	{
		if ((new_user = accept(this->_fd_server, (struct sockaddr*) &this->_addr,(socklen_t*) &this->_addr_len)) < 0)
			errorMsg("accept");
		Client *cl = new Client();
		this->_list_client.insert(std::pair<int, Client*>(new_user, cl));
		this->_list_client[new_user]->set_fd(new_user);
		std::cout << std::endl;
		std::cout <<"===================================" << std::endl;
		std::cout << Colored <<" [~New client connected~] [ID: "<< new_user << "]" << Color << std::endl;
		std::cout << "===================================" << std::endl;
	}
}

/*--------------------------------------------------------*/

void	Server::serverIrc()
{
	while (true)
	{
		char	buffer[1024] = { 0 };
		int		user_talk;
		 
		acceptUser();
		
		for (std::map<int, Client *>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
		{
			user_talk = it->first;
			if (FD_ISSET(user_talk, &this->_fds))
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
				if(bytes_recv <= 0)
				{
					close(user_talk);
					delete this->_list_client[user_talk]->get_user();
					this->_list_client.erase(user_talk);
					std::cout << "Client " << user_talk << " has been disconnected." << std::endl;
				}
				if (strncmp(buffer, "", 1) != 0)
				{
					displayMsgOnServer(buffer, user_talk);
					userSendMsg(buffer, user_talk);
				}
			}
		}
	}
}