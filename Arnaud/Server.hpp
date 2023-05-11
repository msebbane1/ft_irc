/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:53:58 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/11 14:39:35 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

class Server
{
	private:
		int					fd_server;
		int					opt;
		struct sockaddr_in	addr;
		int					addrlen;
		std::vector<int>	list_cl;
	public:
		Server(int port);
		virtual	~Server();
		
		void				acceptConnection();
		struct sockaddr_in	getAddr();
		void				selectServ();
		//void				ft_supp_tab_elem(int **tab, int index, int size);
};