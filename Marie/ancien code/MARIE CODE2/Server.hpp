/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/16 16:20:37 by msebbane         ###   ########.fr       */
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
		fd_set						_fds;
		std::string					_password;
		std::map<int, Client*>		_list_client;
		std::vector<std::string>	_historic;
		std::vector<int>			_fd_users;
	public:
		Server();
		virtual	~Server();

		void						createServ(int port);
		void						errorMsg(std::string msg);
		void						displayMsgOnServer(std::string const &buf, int user_talk);
		void						userSendMsg(std::string const &buf, int user_talk);
		void						acceptUser();
		void						serverIrc();
		void						sendHistoric(int client_fd);
		bool						passTry(std::vector<std::string> line, int cl);
		std::vector<std::string>	splitCustom(std::string buf, char charset);
		void						analyseCommandIrc(std::string buf, int cl);
		int							received(char *buffer, int user_talk);

		void						clientDisconnected();

		std::string	getPassword();
		void		setPassword(std::string pwd);
};