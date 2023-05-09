/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:43:36 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/09 12:06:31 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int argc, char **argv)
{
	int	port;
	
	if (argc != 3)
	{
		std::cout << "Error: Usage : ./ircserv <port> <password> " << std::endl;
		exit(EXIT_FAILURE);
	}

	port = atoi(argv[1]);
	Server *s = new Server(port);
	s->selectServ();
}