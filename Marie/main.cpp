/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:56:14 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/20 16:41:46 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "Color.hpp"

/*Verifie Password valid*/



/*Verifie port valid : digit et compris entre 0 à 65535*/
bool check_adressIP(char *port)
{
    int ip;
    for(int i = 0; port[i]; i++)
    {
        if(!isdigit(port[i]))
            return(false);
    }
    ip = atoi(port);
    if (ip <= 0 || ip > 65535)
        return(false);
    return(true);
}

int main(int argc, char **argv)
{
	Server irc;
	
    if (argc != 3)
		irc.errorMsg("Error: Usage : ./ircserv <port> <password> ");
    if (check_adressIP(argv[1]) == true)
        irc.errorMsg("Error: port [not valid] range betwen 0 and 65535");
    irc.createServ(atoi(argv[1]));
	irc.setPassword(std::string(argv[2]));
    std::cout << Blue << "Waiting for connections..." << Color << std::endl;
	irc.serverIrc();
}