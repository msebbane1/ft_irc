/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/22 10:22:58 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include "Client.hpp"
#include <fcntl.h>
#include <vector>
#include "Channel.hpp"
#include "Commands.hpp"
#include "Color.hpp"

class Commands;
class Client;

class Server
{
	protected:
		struct sockaddr_in			_addr;
		int							_addr_len;
		int							_fd_server;
		fd_set						_fds;
		std::string					_password;
		std::vector<std::string>	_command_list;
		
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
		std::vector<std::string>	splitCustom2(std::string buf, char charset);
		int							received(char *buffer, int user_talk);
		void						clientDisconnected();
		bool						isCommandIrc(std::string str);
		int							countCharInString(std::string buf, char c);
		void						connectToNetCat(int user_talk, std::string buf);
		void						chanExist(std::string name);
		std::vector<int>			_fd_users_dc; // a mettre em prive 
		std::vector<std::string>	_historic;
		std::map<int, Client*>		_list_client;
		
		std::vector<int>			getFdUsersDc();
		void						setFdUsersDc(std::vector<int> fdUsersDc);
		
		std::string					getPassword();
		void						setPassword(std::string pwd);
};