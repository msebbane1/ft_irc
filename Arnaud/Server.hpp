/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:53:58 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/07 17:40:56 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

class Server
{
	private:
		int					fd_server;
		int					new_socket;
		int					opt;
		struct sockaddr_in	addr;
		int					addrlen;
	public:
		Server(int port);
		virtual	~Server();
		
		size_t				ft_recv(char* buffer, size_t size);
		void				acceptConnection();
		struct sockaddr_in	getAddr();
};