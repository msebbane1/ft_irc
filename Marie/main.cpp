/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:56:14 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/22 11:43:45 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*Verifie Password valid*/



/*Verifie port valid : digit et compris entre 0 à 65535*/
bool check_adressIP(char *port)
{
    int ip;
    for(int i = 0; port[i]; i++)
    {
        if(!isdigit(port[i]))
            return(true);
    }
    ip = atoi(port);
    if (ip <= 0 || ip > 65535)
        return(true);
    return(false);
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