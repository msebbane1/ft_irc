/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/19 13:42:39 by asahonet         ###   ########.fr       */
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
#include "Channel.hpp"
#include <fcntl.h>
#include <vector>
#include "Color.hpp"
class Client;
class Channel;

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
		std::vector<int>			_fd_users_dc;
		std::vector<std::string>	_command_list;
		std::vector<Channel*>		_list_chan;
	public:
		Server();
		virtual	~Server();

		void						createServ(int port);
		void						errorMsg(std::string msg);
		void						displayMsgOnServer(std::string const &buf, int user_talk);
		void						userSendMsg(std::string const &buf, int user_talk, std::string name_chan);
		void						acceptUser();
		void						serverIrc();
		bool						connectToNc(std::vector<std::string> line, int cl);
		std::vector<std::string>	splitCustom(std::string buf, char charset);
		void						analyseCommandIrc(std::string buf, int cl);
		int							received(char *buffer, int user_talk);
		void						clientDisconnected();
		bool						isCommandIrc(std::string str);
		int							countCharInString(std::string buf, char c);
		bool						chanExist(std::string name);
		Channel						*takeServ(std::string name);
		bool						userIsInChan(std::string name_chan, int fd_user);

		std::string	getPassword();
		void		setPassword(std::string pwd);
};