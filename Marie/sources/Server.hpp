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

///////DEFINES//////
#define VALID 1
#define INVALID 0

class Client;

class Server
{
    public :
        Server();
        virtual ~Server();

        void Error_msg(std::string msg);
        void Creation_server_irc(int port);
        void Connection_users(Client *user);
		void Display_msg_on_server(std::string const &buf);
		void User_send_msg(std::string const &buf);
		void Accept_users(Client *user);

        std::string get_password();
        void        set_password(std::string password);

        std::map<int, Client * > list_client;

    private :
		struct sockaddr_in	_address;//la structure d'adresse pour le socket
		int	_addrlen; //taille
        int _fd_socket;//pour stocker le descripteur de fichier du socket du serveur
        int _new_socket;//le nouveau socket pour la connexion entrante
        int _valread;//le nombre de caractères lus ou écrits
		int	_user_fd_talk; // fd de celui qui envoie
		int _fd_received; // fd de ceux qui recoivent
		fd_set _fds;
		std::string _password;
};
