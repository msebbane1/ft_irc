/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:29 by asahonet          #+#    #+#             */
/*   Updated: 2023/05/19 13:02:47 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
class Server;
#include "Client.hpp"
class Client;
#include <vector>

/**
 * Commande a gerer dans le channel:
 * 	- KICK
 * 	- MODE
 * 	- JOIN
 * 	- PART
 * 	- PRIVMSG
 * 	- NOTICE
 * 	- QUIT
*/
class Channel
{
	private:
		std::string					_name;
		std::map<int, Client *>		_list_user_co;
		//std::vector<int>			_fd_users_dc;
		std::vector<int>			_list_banned;
		std::string					_password;
		//std::vector<std::string>	_historic;
		
	public:
		Channel(std::string name);
		Channel(std::string name, std::string pw);
		virtual	~Channel();

		void	addUser(Client *cl, int fd_cl);
		void	removeUser(std::string username);
		void	banUser(std::string username);
		void	displayUsers();
		
		std::vector<std::string>	getHisto();
		void						addHisto(std::string msg);
		std::string					getName();
		void						setName(std::string name);
		std::map<int, Client*>		getListUserCo();
};