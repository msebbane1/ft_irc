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

///////DEFINES//////
#define VALID 1
#define INVALID 0
#define PORT 8080 // sera le port utilisé par le serveur pour les connexions entrantes.

class Server
{
    public :
        Server();
        virtual ~Server();

        void creation_server_irc(int port);
        void Error_msg(std::string msg);

    private :
		struct sockaddr_in	_address;//la structure d'adresse pour le socket
		int	_addrlen;
        int _fd_socket;//pour stocker le descripteur de fichier du socket du serveur
        int _new_socket;//le nouveau socket pour la connexion entrante
        int _valread;//le nombre de caractères lus ou écrits
        char _buffer[1024] = { 0 };// Une chaîne de caractères buffer est déclarée pour stocker les données lues à partir de la connexion entrante. 
};
