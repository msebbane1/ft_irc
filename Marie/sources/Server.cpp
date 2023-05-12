#include "Server.hpp"

Server::Server()
{
}

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
    std::cout << Red << msg << Color << std::endl;
	exit(EXIT_FAILURE);
}

void Server::Creation_server_irc(int port)
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
	std::cout << Blue << "Listen to port : " << port << Color << std::endl;
}

void Server::Display_msg_on_server(std::string const &buf)
{
	// verifier plusieurs cas 
	if(buf == "\n")
		return;
    std::cout << "| USER : client " << _user_fd_talk << " |" << std::endl;
    std::cout << "Message send :" << buf ;
}

std::vector<std::string>  Server::customSplit(std::string str, char separator) 
{
    unsigned long startIndex = 0;
	unsigned long endIndex = 0;
	std::vector<std::string> tmpsplit;
    for (unsigned long i = 0; i <= str.size(); i++)
	{
        if (str[i] == separator || i == str.size()) 
		{
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            tmpsplit.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
	return (tmpsplit);
}

void Server::connectToNetcat(std::string buf, int fd_received)
{
	buf.erase(buf.length() - 1);
	std::vector<std::string> splitbuf;
	splitbuf = customSplit(buf, ' ');
	//for(std::vector<std::string>::iterator it = splitbuf.begin(); it != splitbuf.end(); it++)
		//std::cout << *it << std::endl;
	if(splitbuf[0] == "PASS")
	{
		std::cout << splitbuf[1];
		std::cout << get_password();
		if(splitbuf[1] == _password)
		{
			std::cout << "password" << std::endl;
		}
		if(splitbuf[1] != _password)
		{
			list_client[fd_received]->increment_pass_try();
			if(list_client[fd_received]->get_pass_try() == 3)
			{
				close(fd_received);
				list_client.erase(fd_received);
			}
			//exit(1);
		}
	}
}

void Server::User_send_msg(std::string buf, int fd_received)
{
	std::string msg;
	std::string msg_error;
	msg = "Message of [ID: " + std::to_string(_user_fd_talk) + "] : " + buf;
	msg_error = "Error : limit char\n";
	
	connectToNetcat(buf, fd_received);
	std::cout << "buf ===== " << buf << std::endl;
	if (buf[0] == '\n' || buf[0] == '\t')
		return ;
	if(buf.length() > 50)
	{
		send(_user_fd_talk, msg_error.c_str(), msg_error.length(), 0);
		return ;
	}
	send(fd_received, msg.c_str(), msg.length(), 0);
		//std::cout << "user fd2 ========" << _fd_received << std::endl; // ceux qui recoivent
	
}

void Server::Accept_users(Client *user)
{
	FD_ZERO(&_fds);
	FD_SET(_fd_socket, &_fds);

	for (std::map<int, Client *>::iterator it = list_client.begin(); it != list_client.end(); it++)
	{
		_user_fd_talk = it->first;
		FD_SET(_user_fd_talk, &_fds);
	}

    if (select(list_client.size() + _fd_socket + 1, &_fds, NULL, NULL, NULL) < 0)
		Error_msg("select");
	if (FD_ISSET(_fd_socket, &_fds))
	{
		std::cout << "pass" << std::endl;
		// accept() : accepte une connexion entrante. Cela bloque l'exécution jusqu'à ce qu'une connexion soit effectuée.
		if ((_new_socket = accept(_fd_socket, (struct sockaddr*)&_address,(socklen_t*)&_addrlen)) < 0)
			Error_msg("accept");
		//fcntl(_new_socket, F_SETFL, O_NONBLOCK); // POUR LA PARTIE NON BLOCANTE
		list_client.insert(std::pair<int, Client *>(_new_socket, user));
		list_client[_new_socket]->set_fd(_new_socket);

		std::cout << std::endl;
		std::cout <<"===================================" << std::endl;
		std::cout << Colored <<" [~New client connected~] [ID: "<< user->get_fd() << "]" << Color << std::endl;
		std::cout << "===================================" << std::endl;
		std::string msg;
		msg = "\033[3;44;30mUser [ID: " + std::to_string(user->get_fd()) + "]: " + "CONNECTED" + Color + "\n";
		send(user->get_fd(), msg.c_str(), msg.length(), 0);
	}
}

void Server::Connection_users(Client *user)
{
	while(true)
	{
		char buffer[1024] = { 0 }; // pour stocker les données lues à partir de la connexion entrante.
		int fd_received;
		Accept_users(user);

		for (std::map<int, Client *>::iterator it = list_client.begin(); it != list_client.end(); it++)
		{
			_user_fd_talk = it->first;
			if (FD_ISSET(_user_fd_talk, &_fds))
			{
				_valread = recv(_user_fd_talk, buffer, 1024, 0);
				if(_valread)
				{
					std::cout << "=============================" << std::endl;
					Display_msg_on_server(buffer);
					for(std::map<int, Client *>::iterator ite = list_client.begin(); ite != list_client.end(); ite++)
					{
						fd_received = ite->first;
						User_send_msg(buffer, fd_received);
					}
				}
				else
				{
					close(_fd_received);
					//Error_msg("error");
				}
			}
			else
			{
				//std::cout << "pass3" << std::endl;
    			//closing the connected socket
    			//close(_new_socket);
    			//closing the listening socket
    			//shutdown(_fd_socket, SHUT_RDWR);
			}
		}
	}
}