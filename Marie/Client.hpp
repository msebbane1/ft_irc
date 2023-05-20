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
#include <fcntl.h>
#include <vector>
#include "Channel.hpp"
#include "Commands.hpp"
#include "Color.hpp"

///////DEFINES//////
#define VALID 1
#define INVALID 0

class Commands;

class Client
{
    public :
        Client();
        ~Client();

		int			get_pass_try();
		void		increment_pass_try();
		void		setNickname(std::string nickname);
		std::string getUser();
		std::string getNickname();
		void		setUser(std::string user);
		bool		isConnected();
		bool		nicknameIsSet();
		bool		userIsSet();
		bool		passwordIsSet();
		void		setPassword();

    private :
		int		_pass_try;
		bool	_password;
		std::string _nickname;
		std::string	_user;
};
