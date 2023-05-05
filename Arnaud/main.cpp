/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:43:36 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/05 13:11:06 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Error: Usage : ./ircserv <port> <password> " << std::endl;
		exit(EXIT_FAILURE);
	}

	Server *s = new Server(atoi(argv[1]));
	s->acceptConnection();
}