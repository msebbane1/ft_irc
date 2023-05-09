/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:22:25 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/09 13:34:43 by asahonet         ###   ########.fr       */
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

/*void				Server::ft_supp_tab_elem(int **tab, int index, int size)
{
	int	*tmp = 0;
	int	i = 0;
	
	while (i < index)
	{
		tmp[i] = *tab[i];
		i++;
	}
	i = index + 1;
	while (i < size)
	{
		tmp[i] = *tab[i];
		i++;
	}
	tab = &tmp;
}*/

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
			//int			client;
			int			bytes_recv;
			//int			bytes_send;
			//std::string	msg;
			
			sock = this->list_cl[i];
			
			if (sock == this->fd_server && FD_ISSET(sock, &read_sockets))
			{
				fd_client = accept(this->fd_server, NULL, NULL);
				this->list_cl.push_back(fd_client);
				std::cout << "New client connected: " << fd_client << std::endl;
			}
			char	buffer[1024] = {0};
			
			bytes_recv = recv(this->new_socket, buffer, 1024, 0);
			printf("from %d: %s", fd_client, buffer);
			if (bytes_recv > 0)
				close(fd_client);
			std::string o = "Message received\n";
			send(fd_client, o.c_str(), o.size(), 0);
			/*for (unsigned long j = 0; j < this->list_cl.size(); j++)
			{
				printf("\nid sock: %d\n", this->list_cl[j]);
			}
			printf("-------------------");*/

			/*else if (FD_ISSET(sock, &read_sockets))
			{
				char	buffer[1024] = {0};
				//bool	bl_n = false;
				
				while(bl_n == false && (bytes_recv += recv(this->new_socket, buffer + bytes_recv, 1024 - bytes_recv - 1, 0)) > 0)
					if (buffer[bytes_recv - 1] == '\n')
						bl_n = true;
				bytes_recv = recv(this->new_socket, buffer, 1024, 0);
				printf("from %d: %s", sock, buffer);
				if (bytes_recv <= 0)
				{
					close(sock);
					this->list_cl.erase(this->list_cl.begin() + i);
					continue ;
				}

				msg = std::to_string(sock) + ": " + buffer;
				for (unsigned long j = 0; j < this->list_cl.size(); j++)
				{
					client = this->list_cl[i];
					if (client != this->fd_server && client != sock)
					{
						bytes_send = send(client, msg.c_str(), msg.size(), 0);
						if (bytes_send < 0)
						{
							close(client);
							this->list_cl.erase(this->list_cl.begin() + j);
							continue ;
						}
					}
				}
			}*/
		}
	}
}
            
/*
// list of connected clients
    vector<int> client_list;
    client_list.push_back(server_socket);

    while (true) {
        // use select() to monitor the sockets for any incoming data
        fd_set read_sockets;
        FD_ZERO(&read_sockets);
        int max_fd = server_socket;
        for (int i = 0; i < client_list.size(); i++) {
            int fd = client_list[i];
            FD_SET(fd, &read_sockets);
            max_fd = max(max_fd, fd);
        }

        int ready = select(max_fd + 1, &read_sockets, NULL, NULL, NULL);
        if (ready < 0) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < client_list.size(); i++) {
            int sock = client_list[i];

            // new connection
            if (sock == server_socket && FD_ISSET(sock, &read_sockets)) {
                int client_socket = accept(server_socket, NULL, NULL);
                client_list.push_back(client_socket);
                cout << "New client connected: " << client_socket << endl;
            }
			
			// incoming data from a client
            else if (FD_ISSET(sock, &read_sockets)) {
                char buffer[1024] = {0};
                int bytes_read = read(sock, buffer, 1024);
                if (bytes_read <= 0) {
                    close(sock);
                    client_list.erase(client_list.begin() + i);
                    continue;
                }

                // process the incoming data and send it back to all clients
                string message = to_string(sock) + ": " + buffer;
                for (int j = 0; j < client_list.size(); j++) {
                    int client = client_list[j];
                    if (client != server_socket && client != sock) {
                        int bytes_sent = send(client, message.c_str(), message.size(), 0);
                        if (bytes_sent == -1) {
                            close(client);
                            client_list.erase(client_list.begin() + j);
                            continue;
                        }
                    }
                }
            }
        }
    }

    return 0;
}*/