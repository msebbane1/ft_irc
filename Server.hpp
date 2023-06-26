/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/22 07:14:33 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <sstream>
#include <fstream>
#include <cstring>
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
#include "Messages.hpp"
#include "Color.hpp"
#include "Commands/Commands.hpp"

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
		std::string					_passwordOper;
		std::vector<std::string>	_command_list;
		std::vector<int>			_fd_users_dc;
		std::map<int, Client*>		_list_client;
		std::map<std::string, Channel*>	_list_chan;
		
	public:
		Server();
		virtual	~Server();

		//=======================Creation Server=====================//
		void						serverIrc();
		void						createServ(int port);
		void						acceptUser();
		int							received(char *buffer, int user_talk);
		void						connectToClients(int user_talk, std::string buf);
		void 						connect(int user_talk, std::string buf);
		void						destroyChannIfEmpty();
		
		//==========================Utils===========================//
		void						sendHistoric(int client_fd);
		int							countCharInString(std::string buf, char c);
		std::vector<std::string>	splitCustom(std::string buf, char charset);
		
		void						clientDisconnected();
		void						channDisconnected();
		bool						isCommandIrc(std::string str);
		bool 						clientExist(std::string nick);
		
		//==========================GETTER && SETTER===========================//
		std::vector<int>			getFdUsersDc();
		void						setFdUsersDc(int fdUsersDc);
		
		std::string					getPassword();
		std::string					getPasswordOper();
		void						setPassword(std::string pwd);

		std::map<int, Client*>		getListClient();
		void						setListClient(int fd, Client *user);
		Client *					getClient(std::string nick);
		
		std::map<std::string, Channel*>	getListChan();
		Channel*						getChannel(std::string chann);
		void							addListChan(Channel *c);
		
};