#include "Server.hpp"
#include "Client.hpp"
#include "Color.hpp"


/*Verifie port valid : digit et compris entre 0 à 65535*/
int check_adressIP(char *port)
{
    int ip;
    for(int i = 0; port[i]; i++)
    {
        if(!isdigit(port[i]))
            return(INVALID);
    }
    ip = atoi(port);
    if (ip <= 0 || ip > 65535)
        return(INVALID);
    return(VALID);
}

int main(int argc, char **argv)
{
	Server irc;
	Client *user = new Client();
	if (argc != 3)
		irc.Error_msg("Error: Usage : ./ircserv <port> <password> ");

    if (check_adressIP(argv[1]) == INVALID)
        irc.Error_msg("Error: port [not valid] range betwen 0 and 65535");

	irc.set_password(std::string(argv[2]));
    irc.Creation_server_irc(atoi(argv[1]));
    std::cout << Blue << "Waiting for connections..." << Color << std::endl;
	irc.Connection_users(user);
}