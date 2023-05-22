/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/22 10:50:55 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

///////INCLUDES//////
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
#include "Server.hpp"
#include <fcntl.h>
#include <vector>
#include "Color.hpp"

class Server;
class Client;
class Channel;

class Commands
{
	private :
		std::vector<Channel*>		_list_chan;
		
	public :
		Commands();
		~Commands();

		void						exec_cmd(Server *server, std::vector<std::string>	cmd, Client *user, int user_talk);
		std::vector<std::string>	splitCustom(std::string buf, char charset);

		bool						chanExist(std::string name);
		Channel						*takeServ(std::string name);
		bool						userIsInChan(std::string name_chan, int fd_user);
		//=========================COMMANDS======================//
		void						cmdToConnect(Server *server, std::vector<std::string> cmd, Client *user, int user_talk);
		bool						passCmd(std::vector<std::string> line, int cl, Client *user, Server *server);
		void						privMsgCmd(int user_talk, Client *user, Server *server, std::vector<std::string> cmd);
};