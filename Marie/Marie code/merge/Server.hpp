/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:40:17 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/16 11:50:35 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
///////INCLUDES//////
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

#define VALID 1
#define INVALID 0

class Client;

class Server
{
	private:
		struct sockaddr_in	_addr;//la structure d'adresse pour le socket
		int	_addrlen; //taille
        int _fd_socket;//pour stocker le descripteur de fichier du socket du serveur
		int	_user_fd_talk; // fd de celui qui envoie
		fd_set _fds;
		int _connected;
		std::string _password;
		std::map<int, Client *>		_list_client;
		std::vector<std::string>	_historic;
	public:
		Server();
		virtual	~Server();

		void		createServerIrc(int port);
		void		errorMsg(std::string msg);
		void		displayMsgOnServer(std::string const &buf);
		void		userSendMsg(std::string const &buf);
		void		acceptUser(Client *user);
		void		serverIrc(Client *user);
		void		sendHistoric(int client_fd);
		void		connectToNetcat(std::string buf, Client *client);
		std::vector<std::string> customSplit(std::string str, char separator);
		void		received(char *buffer);

		std::string	getPassword();
		void		setPassword(std::string pwd);
};