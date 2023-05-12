/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/12 11:47:33 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include "Client.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include "Color.hpp"
class Client;

#define VALID 1
#define INVALID 0

class Server
{
	private:
		struct sockaddr_in			_addr;
		int							_addr_len;
		int							_fd_server;
		int							_user_fd_talk;
		int							_fd_received;
		fd_set						_fds;
		std::string					_password;
		std::map<int, Client*>		_list_client;
		std::vector<std::string>	_historic;
	public:
		Server(int port);
		virtual	~Server();

		void		errorMsg(std::string msg);
		void		displayMsgOnServer(std::string const &buf);
		void		userSendMsg(std::string const &buf);
		void		acceptUser(Client *user);
		void		serverIrc(Client *user);
		void		sendHistoric(int client_fd);

		std::string	getPassword();
		void		setPassword(std::string pwd);
};