/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/20 15:58:56 by msebbane         ###   ########.fr       */
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
#include "Server.hpp"
#include <fcntl.h>
#include <vector>
#include "Color.hpp"

class Server;
class Client;

class Commands
{
	public :
		Commands();
		~Commands();

		void						exec_cmd(Server *server, std::string buf, Client *user, int user_talk);
		std::vector<std::string>	splitCustom(std::string buf, char charset);
		//=========================COMMANDS======================//
		bool						passCmd(std::vector<std::string> line, int cl, Client *user, Server *server);
		void						privMsgCmd(int user_talk, Client *user, Server *server, std::vector<std::string> cmd);
};