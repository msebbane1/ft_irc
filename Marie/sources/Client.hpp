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

///////DEFINES//////
#define VALID 1
#define INVALID 0

//Class Client;

class Client
{
    public :
        Client();
        ~Client();

		Client * get_user();
		void	set_user(Client *user);

    private :
		Client *user;

};