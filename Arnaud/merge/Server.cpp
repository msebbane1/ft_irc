/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:41:57 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/12 12:32:49 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port)
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

Server::~Server()
{
}

std::string	Server::getPassword()
{
    return (this->_password);
}

void		Server::setPassword(std::string pwd)
{
    this->_password = pwd;
}

void		Server::errorMsg(std::string msg)
{
    std::cout << Red << msg << Color << std::endl;
	exit(EXIT_FAILURE);
}

void		Server::displayMsgOnServer(std::string const &buf)
{
	if(buf == "\n")
		return;
    std::cout << "| USER : client " << _user_fd_talk << " |" << std::endl;
    std::cout << "Message send :" << buf ;
}

void		Server::userSendMsg(std::string const &buf)
{
	std::string	msg;
	int			fd_received;
	
	msg = "Message of [ID: " + std::to_string(this->_user_fd_talk) + "] : " + buf;
	if (buf[0] == '\n' || buf[0] == '\t')
		return ;
	for(std::map<int, Client *>::iterator ite = this->_list_client.begin(); ite != this->_list_client.end(); ite++)
	{
		fd_received = ite->first;
		if (fd_received != this->_user_fd_talk)
		{
			if (send(fd_received, msg.c_str(), msg.length(), 0) < 0)
			{
				std::cout << "Send failed" << std::endl;
				close(this->_user_fd_talk);
				this->_list_client.erase(this->_user_fd_talk);
				std::cout << "Client " << this->_user_fd_talk << " has been disconnected." << std::endl;
			}
		}
	}
	if (this->_list_client[this->_user_fd_talk])
		this->_historic.push_back(msg);
}

void		Server::sendHistoric(int client_fd)
{
	for (unsigned int i = 0; i < this->_historic.size(); i++)
		send(client_fd, this->_historic[i].c_str() , this->_historic[i].size(), 0);
}

void		Server::acceptUser(Client *user)
{
	int	max_fd;
	int	new_user;
	
	FD_ZERO(&this->_fds);
	FD_SET(this->_fd_server, &this->_fds);
	
	max_fd = this->_fd_server;
	for (std::map<int, Client *>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		this->_user_fd_talk = it->first;
		FD_SET(this->_user_fd_talk, &this->_fds);
		fcntl(this->_user_fd_talk, O_NONBLOCK);
		max_fd = std::max(max_fd, this->_user_fd_talk);
	}

	if (select(max_fd + 1, &this->_fds, NULL, NULL, NULL) < 0)
		errorMsg("select");
	if (FD_ISSET(this->_fd_server, &this->_fds))
	{
		if ((new_user = accept(this->_fd_server, (struct sockaddr*) &this->_addr,(socklen_t*) &this->_addr_len)) < 0)
			errorMsg("accept");
		this->_list_client.insert(std::pair<int, Client*>(new_user, user));
		this->_list_client[new_user]->set_fd(new_user);
		if (this->_historic.size() > 1)
			sendHistoric(new_user);
		std::cout << std::endl;
		std::cout <<"===================================" << std::endl;
		std::cout << Colored <<" [~New client connected~] [ID: "<< user->get_fd() << "]" << Color << std::endl;
		std::cout << "===================================" << std::endl;
	}
}

void		Server::serverIrc(Client *user)
{
	while (true)
	{
		char	buffer[1024] = { 0 };

		acceptUser(user);

		for (std::map<int, Client *>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
		{
			this->_user_fd_talk = it->first;
			if (FD_ISSET(this->_user_fd_talk, &this->_fds))
			{
				int		n = 0;
				int		bytes_recv = 0;
				bool	bl_n = false;
				
				while (bl_n == false && (n = recv(this->_user_fd_talk, buffer + bytes_recv, 1024 - bytes_recv - 1, 0)) > 0)
				{
					bytes_recv += n;
					if (buffer[bytes_recv - 1] == '\n')
						bl_n = true;
				}
				buffer[bytes_recv] = 0;
				if(bytes_recv <= 0)
				{
					close(this->_user_fd_talk);
					this->_list_client.erase(this->_user_fd_talk);
					std::cout << "Client " << this->_user_fd_talk << " has been disconnected." << std::endl;
				}
				if (strncmp(buffer, "", 1) != 0)
				{
					displayMsgOnServer(buffer);
					userSendMsg(buffer);
				}
			}
		}
	}
}