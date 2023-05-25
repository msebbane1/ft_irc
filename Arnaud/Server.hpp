/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/24 10:23:50 by asahonet         ###   ########.fr       */
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

		//=======================Creation Server=====================//
		void						serverIrc();
		void						createServ(int port);
		void						acceptUser();
		int							received(char *buffer, int user_talk);
		void						connectToNetCat(int user_talk, std::string buf);
		void						connectToIRSSI(int user_talk, std::string buf);
		void 						connect(int user_talk, std::string buf);
		
		//==========================Utils===========================//
		void						displayMsgOnServer(std::string const &buf, int user_talk);
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
		
		std::vector<Channel*>		getListChan();
		void						addListChan(Channel *c);
		
		//===========================ERROR MSG && MSG==============================//
		void	errorMsg(std::string msg);
		void	errorSendBuf(std::string num, std::string nick, std::string arg, std::string msg, int fd);
		void	errorSend(std::string num, std::string nick, std::string msg, int fd);
		void	welcomeMsg(std::string user, std::string nick, int fd);
};