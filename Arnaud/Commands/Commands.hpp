/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/06 13:48:51 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

///////INCLUDES//////
#include "../Server.hpp"

class Server;
class Messages;
class Channel;
class Client;

class Commands
{
	private :
		Messages*					_msg;
		Server* 					_s;
		Client*						_user;
		int							_fd_user;
		std::vector<std::string>	_line_cmd;

	public :
		Commands(Server *s, Client *c, int fd_c, std::vector<std::string> linecmd, Messages msg);
		~Commands();

		void						exec_cmd();

		bool						chanExist(std::string name_chan);
		
		//=========================COMMANDS======================//
		void						cmdToConnect();
		void						passCmd();
		void						userCmd();
		void						nickCmd();
		void						privMsgCmd();
		std::string					joinMessages();
		void						joinCmd();
		void						quitCmd();
		void						operCmd();
		void						killCmd();
		void						inviteCmd();
};