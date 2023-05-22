/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/22 13:30:28 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

///////INCLUDES//////
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Client;
class Server;
class Server;

class Commands
{
	private:
		Server* 					_s;
		Client*						_user;
		int							_fd_user;
		std::string					_cmd;
		std::vector<std::string>	_line_cmd;

	public :
		Commands(Server *s, Client *c, int fd_c, std::string cmd, std::vector<std::string> linecmd);
		~Commands();

		void						exec_cmd();
		std::vector<std::string>	splitCustom(std::string buf, char charset);

		bool						chanExist(std::string name);
		Channel						*takeServ(std::string name);
		bool						userIsInChan(std::string name_chan, int fd_user);
		//=========================COMMANDS======================//
		void						cmdToConnect(Server *server, std::vector<std::string> cmd, Client *user, int user_talk);
		bool						passCmd(std::vector<std::string> line, int cl, Client *user, Server *server);
		void						privMsgCmd(int user_talk, Client *user, Server *server, std::vector<std::string> cmd);
};