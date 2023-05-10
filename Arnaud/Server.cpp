/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:22:25 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/10 13:44:58 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port)
{
	this->opt = 1;
	if ((this->fd_server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(this->fd_server, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(this->opt)) == -1)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = INADDR_ANY;
	this->addr.sin_port = htons(port);

	if (bind(this->fd_server, (struct sockaddr*) &this->addr, sizeof(this->addr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	this->addrlen = sizeof(this->addr);
	
	if (listen(this->fd_server, 500) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	this->list_cl.push_back(this->fd_server);
}

Server::~Server()
{
}

/*void	Server::acceptConnection()
{
	if ((this->new_socket = accept(this->fd_server, (struct sockaddr*) &this->addr, (socklen_t*) &this->addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	char	buffer[1024] = {0};
	int		tot_c = 0;
	bool	bl_n = false;
	
    while(bl_n == false && (tot_c += recv(this->new_socket, buffer + tot_c, 1024 - tot_c - 1, 0)) > 0)
		if (buffer[tot_c - 1] == '\n')
			bl_n = true;
	buffer[tot_c] = 0;
	std::cout << buffer << std::endl;
	const char *hello = "Hello from server";
	send(this->new_socket, hello, strlen(hello), 0);
	std::cout << "Hello message sent" << std::endl;
	close(this->new_socket);
}*/

struct sockaddr_in	Server::getAddr()
{
	return (this->addr);
}

void				Server::selectServ()
{
	while (true)
	{
		fd_set	read_sockets;
		int		max_fd;
		int 	fd;
		
		max_fd = this->fd_server;
		FD_ZERO(&read_sockets);
		for (unsigned long i = 0; i < this->list_cl.size(); i++)
		{
			fd = this->list_cl[i];
			FD_SET(fd, &read_sockets);
			max_fd = std::max(max_fd, fd);
		}

		int	slct = select(max_fd + 1, &read_sockets, NULL, NULL, NULL);
		if (slct < 0)
		{
			perror("select failed");
			exit(EXIT_FAILURE);
		}

		for (unsigned long i = 0; i < this->list_cl.size(); i++)
		{
			int			sock;
			int			fd_client = 0;
			int			client;
			int			bytes_recv = 0;
			int			bytes_send;
			std::string	msg;
			
			sock = this->list_cl[i];
			
			if (sock == this->fd_server && FD_ISSET(sock, &read_sockets))
			{
				fd_client = accept(this->fd_server, NULL, NULL);
				this->list_cl.push_back(fd_client);
				std::cout << "New client connected: " << fd_client << std::endl;
			}
			else if (FD_ISSET(sock, &read_sockets))
			{
				char	buffer[1024] = {0};
				
				bytes_recv = recv(sock, buffer, 1024, 0);
				if (bytes_recv <= 0)
				{
					close(sock);
					this->list_cl.erase(this->list_cl.begin() + i);
				}

				msg = std::to_string(sock) + ": " + buffer;
				for (unsigned long j = 0; j < this->list_cl.size(); j++)
				{
					client = this->list_cl[j];
					if (client != sock)
					{
						bytes_send = send(client, msg.c_str(), msg.size(), 0);
						if (bytes_send < 0)
						{
							close(client);
							this->list_cl.erase(this->list_cl.begin() + j);
						}
					}
				}
			}
		}
	}
}