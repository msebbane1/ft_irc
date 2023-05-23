/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/23 10:50:26 by asahonet         ###   ########.fr       */
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
#include <fcntl.h>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"
#include "Commands.hpp"
#include "Color.hpp"

class Commands;
class Client;
class Channel;

class Server
{
	private:
		struct sockaddr_in			_addr;
		int							_addr_len;
		int							_fd_server;
		fd_set						_fds;
		std::string					_password;
		std::vector<std::string>	_command_list;
		std::vector<int>			_fd_users_dc;
		std::map<int, Client*>		_list_client;
		std::vector<Channel*>		_list_chan;
		
	public:
		Server();
		virtual	~Server();

		void						serverIrc();
		void						createServ(int port);
		void						acceptUser();
		int							received(char *buffer, int user_talk);
		void						connectToNetCat(int user_talk, std::string buf);
		
		void						displayMsgOnServer(std::string const &buf, int user_talk);
		void						sendHistoric(int client_fd);
		void						errorMsg(std::string msg);
		int							countCharInString(std::string buf, char c);
		std::vector<std::string>	splitCustom(std::string buf, char charset);
		
		void						clientDisconnected();
		bool						isCommandIrc(std::string str);
		void						chanExist(std::string name);
		
		std::vector<int>			getFdUsersDc();
		void						setFdUsersDc(int fdUsersDc);
		
		std::string					getPassword();
		void						setPassword(std::string pwd);

		std::map<int, Client*>		getListClient();
		
		std::vector<Channel*>		getListChan();
		void						addListChan(Channel *c);
};