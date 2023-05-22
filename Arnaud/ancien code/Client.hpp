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
#include "Color.hpp"

///////DEFINES//////
#define VALID 1
#define INVALID 0

//Class Client;

class Client
{
    public :
        Client();
        ~Client();

		std::string	getUser();
		std::string	getNickname();
		int			get_pass_try();
		void		increment_pass_try();
		bool		getConnected();
		void		setConnected();
		void		setNickname(std::string nickname);
		void		setUser(std::string user);
		bool		isConnected();
		bool		nicknameIsSet();
		bool		userIsSet();

    private :
		int		_pass_try;
		bool	_connected;
		std::string _nickname;
		std::string	_user;
};