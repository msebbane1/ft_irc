#include "Server.hpp"
#include "Client.hpp"

int main(int argc, char **argv)
{
	//Creation du serveur []
	if (argc != 3)
	{
		std::cout << "Error: Usage : ./ircserv <port> <password> " << std::endl;
		exit();
		return (1);
	}
	//Creer un password () <password> getpassword recuperer password pour l'utiliser plus tard std::string
	//check port input // adresse ip parse num
	//serveur
	start_server(&server, av[1]);
}