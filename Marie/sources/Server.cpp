#include "Server.hpp"

Server::Server(){}

Server::~Server(){}

std::string Server::get_password()
{
    return (_password);
}

void Server::set_password(std::string password)
{
    this->_password = password;
}

void Server::Error_msg(std::string msg)
{
    std::cout << msg << std::endl;
	exit(EXIT_FAILURE);
}

void Server::creation_server_irc(int port)
{
    int opt = 1; // indicateur d'option
    /* Creation socket file descriptor :
    AF_INET = protocoles IPv4, 
    SOCK_STREAM [type de socket] = TCP
    protocole à 0, permet au système d'utiliser le protocole par défaut.
    */
    if ((_fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Error_msg("socket failed");
  
    /*
    Configuration du socket. Ici, les options SO_REUSEADDR et SO_REUSEPORT 
    sont utilisées pour permettre la réutilisation rapide de l'adresse et du port 
    par le système.
    */
    if (setsockopt(_fd_socket, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)) < 0)
        Error_msg("setsockopt");

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(port);
  
    // bind() : lie le socket à l'adresse et au port spécifiés.
    if (bind(_fd_socket, (struct sockaddr*)&_address, sizeof(_address)) < 0)
        Error_msg("bind failed");
    // listen() : met le socket en mode d'écoute, permettant ainsi aux connexions entrantes d'être acceptées
    if (listen(_fd_socket, 3) < 0)
        Error_msg("listen");
}

void Server::add_client()
{
   // fd_set fds;
    //AJOUTER FD_ZERO FD_SET pour les clients
    //creation client???
	char buffer[1024] = { 0 };
    const char *hello;
	hello = "Hello from server";

    //if (select(client_list.size() + _fd_socket + 1, &fds, NULL, NULL, NULL) < 0)
		//Error_msg("select");
    // accept() : accepte une connexion entrante. Cela bloque l'exécution jusqu'à ce qu'une connexion soit effectuée.
    if ((_new_socket = accept(_fd_socket, (struct sockaddr*)&_address,(socklen_t*)&_addrlen)) < 0)
        Error_msg("accept");

    _valread = read(_new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(_new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
  
    // closing the connected socket
    close(_new_socket);
    // closing the listening socket
    shutdown(_fd_socket, SHUT_RDWR);
}