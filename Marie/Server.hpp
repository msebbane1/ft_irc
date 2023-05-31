/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/31 12:20:22 by msebbane         ###   ########.fr       */
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
#include "Messages.hpp"

class Commands;
class Client;
class Channel;
class Messages;

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
		std::map<std::string, Channel*>	_list_chan;
		bool						_irssi;
		
	public:
		Server();
		virtual	~Server();

		//=======================Creation Server=====================//
		void						serverIrc();
		void						createServ(int port);
		void						acceptUser();
		int							received(char *buffer, int user_talk);
		//void						connectToNetCat(int user_talk, std::string buf);
		//void						connectToIRSSI(int user_talk, std::string buf);
		void						connectToClients(int user_talk, std::string buf);
		void 						connect(int user_talk, std::string buf);
		
		//==========================Utils===========================//
		void						sendHistoric(int client_fd);
		int							countCharInString(std::string buf, char c);
		std::vector<std::string>	splitCustom(std::string buf, char charset);
		
		void						clientDisconnected();
		bool						isCommandIrc(std::string str);
		void						chanExist(std::string name);
		bool 						clientExist(std::string nick);
		
		//==========================GETTER && SETTER===========================//
		std::vector<int>			getFdUsersDc();
		void						setFdUsersDc(int fdUsersDc);
		
		std::string					getPassword();
		void						setPassword(std::string pwd);

		std::map<int, Client*>		getListClient();
		void						setListClient(int fd, Client *user);
		Client *					getClient(std::string nick);
		
		std::map<std::string, Channel*>	getListChan();
		void							addListChan(Channel *c);
		
};