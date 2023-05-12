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

		Client		*get_user();
		void		set_user(Client *user);
		void		set_fd(int const &fd);
		int const &	get_fd() const;
		int			get_pass_try();
		void		increment_pass_try();
		bool		getConnected();

    private :
		Client	*user;
		int		_fd;
		int		_pass_try;
		bool	_connected;
};
