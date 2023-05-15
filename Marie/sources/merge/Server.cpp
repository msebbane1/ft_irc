/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:41:57 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/15 15:23:04 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	_connected = false;
}

void Server::createServerIrc(int port)
{
    int opt = 1; // indicateur d'option
    /* Creation socket file descriptor :
    AF_INET = protocoles IPv4, 
    SOCK_STREAM [type de socket] = TCP
    protocole à 0, permet au système d'utiliser le protocole par défaut.
    */
    if ((_fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        errorMsg("socket failed");
  
    /*
    Configuration du socket. Ici, les options SO_REUSEADDR et SO_REUSEPORT 
    sont utilisées pour permettre la réutilisation rapide de l'adresse et du port 
    par le système.
    */
    if (setsockopt(_fd_socket, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)) < 0)
        errorMsg("setsockopt");

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);
  
    // bind() : lie le socket à l'adresse et au port spécifiés.
    if (bind(_fd_socket, (struct sockaddr*)&_addr, sizeof(_addr)) < 0)
        errorMsg("bind failed");
    // listen() : met le socket en mode d'écoute, permettant ainsi aux connexions entrantes d'être acceptées
    if (listen(_fd_socket, 2) < 0)
        errorMsg("listen");
	std::cout << Blue << "Listen to port : " << port << Color << std::endl;
}

Server::~Server(){}

std::string	Server::getPassword()
{
    return (this->_password);
}

void		Server::setPassword(std::string pwd)
{
    this->_password = pwd;
}

void		Server::errorMsg(std::string msg)
{
    std::cout << Red << msg << Color << std::endl;
	exit(EXIT_FAILURE);
}

void		Server::displayMsgOnServer(std::string const &buf)
{
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
	std::string	msg;
	splitbuf = customSplit(buf, ' ');
	
	if (splitbuf.size() >= 2)
	{
		if(splitbuf[0] == "PASS")
		{
			if(splitbuf[1] != _password && _list_client[fd_received]->getConnected() == false)
			{
				msg = "Wrong password : try = " + std::to_string(_list_client[fd_received]->get_pass_try()) + "\n";
				send(this->_user_fd_talk, msg.c_str(), msg.size(), 0);
				_list_client[fd_received]->increment_pass_try();
				if(_list_client[fd_received]->get_pass_try() == 3)
				{
					close(fd_received);
					_list_client.erase(fd_received);
				}
			}
			if(splitbuf[1] == _password)
			{
				std::cout << "password pass" << std::endl;
				_list_client[fd_received]->setConnected(true);
			}
		}	
	}
	else if(splitbuf.size() >= 2 || _list_client[fd_received]->getConnected() == false)
	{
		msg = "\x1B[31mYou need to enter : PASS - NICK - USER\033[0m\n";
		send(this->_user_fd_talk, msg.c_str(), msg.size(), 0);
	}
	

}

void		Server::userSendMsg(std::string const &buf)
{

	int			fd_received;
	std::string	msg;


	for(std::map<int, Client *>::iterator ite = this->_list_client.begin(); ite != this->_list_client.end(); ite++)
	{
		fd_received = ite->first;
		if (buf[0] == '\n' || buf[0] == '\t')
			return ;
		connectToNetcat(buf, _user_fd_talk);
		if(_list_client[_user_fd_talk]->getConnected() == true && _connected == false)
		{
			msg = "\033[3;44;30mUser [ID: " + std::to_string(this->_user_fd_talk) + "]: " + "CONNECTED" + Color + "\n";
			send(this->_user_fd_talk, msg.c_str(), msg.length(), 0);
			_connected = true;
		}
		else if(_list_client[_user_fd_talk]->getConnected() == true)
		{
			msg = "Message of [ID: " + std::to_string(this->_user_fd_talk) + "] : " + buf;
			if (send(fd_received, msg.c_str(), msg.length(), 0) < 0)
				{
					std::cout << "Send failed" << std::endl;
					close(this->_user_fd_talk);
					this->_list_client.erase(this->_user_fd_talk);
					std::cout << "Client " << this->_user_fd_talk << " has been disconnected." << std::endl;
				}
		}


	}
	//if (this->_list_client[this->_user_fd_talk])
		//this->_historic.push_back(msg);
}

void		Server::sendHistoric(int client_fd)
{
	for (unsigned int i = 0; i < this->_historic.size(); i++)
		send(client_fd, this->_historic[i].c_str() , this->_historic[i].size(), 0);
}

void		Server::acceptUser(Client *user)
{
	int	new_fd_user;
	
	FD_ZERO(&this->_fds);
	FD_SET(this->_fd_socket, &this->_fds);
	
	for (std::map<int, Client *>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
	{
		this->_user_fd_talk = it->first;
		FD_SET(this->_user_fd_talk, &this->_fds);
		fcntl(this->_user_fd_talk, O_NONBLOCK);
	}

	if (select(_list_client.size() + _fd_socket + 1, &this->_fds, NULL, NULL, NULL) < 0)
		errorMsg("select");
	if (FD_ISSET(this->_fd_socket, &this->_fds))
	{
		if ((new_fd_user = accept(this->_fd_socket, (struct sockaddr*) &this->_addr,(socklen_t*) &this->_addrlen)) < 0)
			errorMsg("accept");
		this->_list_client.insert(std::pair<int, Client*>(new_fd_user, user));
		this->_list_client[new_fd_user]->set_fd(new_fd_user);
		//if (this->_historic.size() > 1)
			//sendHistoric(new_fd_user);
		
		std::cout << std::endl;
		std::cout <<"===================================" << std::endl;
		std::cout << Colored <<" [~New client connected~] [ID: "<< user->get_fd() << "]" << Color << std::endl;
		std::cout << "===================================" << std::endl;

	}
}

void		Server::received(char *buffer)
{
	int		n = 0;
	int		bytes_recv = 0;
	bool	bl_n = false;
				
	while (bl_n == false && (n = recv(this->_user_fd_talk, buffer + bytes_recv, 1024 - bytes_recv - 1, 0)) > 0)
	{
		bytes_recv += n;
		if (buffer[bytes_recv - 1] == '\n')
			bl_n = true;
	}
	buffer[bytes_recv] = 0;
	if(bytes_recv <= 0)
	{
		close(this->_user_fd_talk);
		this->_list_client.erase(this->_user_fd_talk);
		std::cout << "Client " << this->_user_fd_talk << " has been disconnected." << std::endl;
	}
}
void		Server::serverIrc(Client *user)
{
	while(true)
	{
		char	buffer[1024] = { 0 }; // pour stocker les données lues à partir de la connexion entrante.
		int valread;
		acceptUser(user);

		for (std::map<int, Client *>::iterator it = this->_list_client.begin(); it != this->_list_client.end(); it++)
		{
			this->_user_fd_talk = it->first;
			if (FD_ISSET(this->_user_fd_talk, &this->_fds))
			{
				//received(buffer); // segfault pour ctrl/c
				valread = recv(_user_fd_talk, buffer, 1024, 0);
				if(valread)
				{
					if (strncmp(buffer, "", 1) != 0)
					{
						displayMsgOnServer(buffer);
						userSendMsg(buffer);
					}
				}
			}
		}
	}
}