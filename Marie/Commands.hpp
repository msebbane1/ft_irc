/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/25 16:01:45 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

///////INCLUDES//////
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Client;
class Server;
class Channel;

class Commands
{
	private :
		Server* 					_s;
		Client*						_user;
		int							_fd_user;
		//std::string					_cmd;
		std::vector<std::string>	_line_cmd;

	public :
		Commands(Server *s, Client *c, int fd_c, std::vector<std::string> linecmd);
		~Commands();

		void						exec_cmd();

		bool						chanExist(std::string name);
		Channel						*takeServ(std::string name);
		bool						userIsInChan(std::string name_chan, int fd_user);
		void						sendToChannel(int user_talk, std::string msg, std::string chan);
		
		//=========================COMMANDS======================//
		void						cmdToConnect();
		void						passCmd();
		void						userCmd();
		void						nickCmd();
		void						privMsgCmd();
		void						joinCmd();

		

		class	NeedMoreParams: public std::exception
		{
			public:
				virtual const char *what() const _NOEXCEPT;
		};
};