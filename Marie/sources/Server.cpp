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
    if (listen(_fd_socket, 2) < 0)
        Error_msg("listen");
}

void Server::display_buff(std::string const &buffer)
{
	// afficher client : FD
	// verifier plusieurs cas 
	if(buffer == "\n")
		return;
    std::cout << "| USER : client " << _user_fd << " |" << std::endl;
    std::cout << "Message send :" << buffer ;
}

void Server::user_send_msg(std::string buf)
{
	std::string hello;
	//Verifier taille du buffer
	if (buf[0] == '\n')
		return ;
	if (send(_new_socket, hello.c_str(), hello.length(), 0) == -1)
		std::cout << "Send failed" << std::endl;
	//std::cout << "| SEND TO CLIENT " << _user_fd << "|" << std::endl;
	//std::cout << hello.substr(0, hello.length()) << std::endl;
	//send(_new_socket, hello.c_str(), hello.length(), 0);
}

void Server::add_client(Client *user)
{
    fd_set fds;
    //AJOUTER FD_ZERO FD_SET pour les clients pour verifier
	char buffer[1024] = { 0 };

	FD_ZERO(&fds);
	FD_SET(_fd_socket, &fds);


	for (std::map<int, Client *>::iterator it = list_client.begin(); it != list_client.end(); it++)
	{
		_user_fd = it->first;
		FD_SET(_user_fd, &fds);
	}
    if (select(list_client.size() + _fd_socket + 1, &fds, NULL, NULL, NULL) < 0)
		Error_msg("select");
	if (FD_ISSET(_fd_socket, &fds))
	{
		// accept() : accepte une connexion entrante. Cela bloque l'exécution jusqu'à ce qu'une connexion soit effectuée.
		if ((_new_socket = accept(_fd_socket, (struct sockaddr*)&_address,(socklen_t*)&_addrlen)) < 0)
			Error_msg("accept");
		fcntl(_new_socket, F_SETFL, O_NONBLOCK); // POUR LA PARTIE NON BLOCANTE
		list_client.insert(std::pair<int, Client *>(_new_socket, user));
		//list_client[_new_socket]->set_fd(_new_socket);
		//for (std::map<int, Client *>::iterator it = list_client.begin(); it != list_client.end(); it++)
			//std::cout << "Client: " << it->first << std::endl;
		std::cout << "[New client connected]" << std::endl;
	}
    
	for (std::map<int, Client *>::iterator it = list_client.begin(); it != list_client.end(); it++)
	{
		_user_fd = it->first;
	}
	if (FD_ISSET(_user_fd, &fds))
	{
		_valread = recv(_new_socket, buffer, 1024, 0);
		if(_valread)
		{
			display_buff(buffer);
			user_send_msg(buffer);
		}
		/*
		else
		{
    		// closing the connected socket
    		close(_new_socket);
    		// closing the listening socket
    		shutdown(_fd_socket, SHUT_RDWR);
		}
		*/
	}
}