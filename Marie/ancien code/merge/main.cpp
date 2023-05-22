/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:56:14 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/15 11:22:37 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		irc.errorMsg("Error: Usage : ./ircserv <port> <password> ");
    if (check_adressIP(argv[1]) == INVALID)
        irc.errorMsg("Error: port [not valid] range betwen 0 and 65535");

	irc.setPassword(std::string(argv[2]));
	irc.createServerIrc(atoi(argv[1]));
    std::cout << Blue << "Waiting for connections..." << Color << std::endl;
	irc.serverIrc(user);
}