/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:22:25 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/07 17:40:33 by asahonet         ###   ########.fr       */
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
	
	if (listen(this->fd_server, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}

Server::~Server()
{
}

size_t Server::ft_recv(char* buffer, size_t size) {
    size_t	total = 0;
	size_t	n = 0;
    while((n = ::recv(this->new_socket, buffer + total, size - total - 1, 0)) > 0) {
        total += n;
    }
    buffer[total] = 0;
    return total;
}

void	Server::acceptConnection()
{
	if ((this->new_socket = accept(this->fd_server, (struct sockaddr*) &this->addr, (socklen_t*) &this->addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	char	buffer[1024] = {0};
	ft_recv(buffer, 1024);
	std::cout << buffer << std::endl;
	const char *hello = "Hello from server";
	send(this->new_socket, hello, strlen(hello), 0);
	std::cout << "Hello message sent" << std::endl;
	close(this->new_socket);
}

struct sockaddr_in	Server::getAddr()
{
	return (this->addr);
}