/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/31 16:28:10 by msebbane         ###   ########.fr       */
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
class Messages;

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
		Channel*					getChannel(std::string name_chan);
		bool						userIsInChan(std::string name_chan, int fd_user);
		void						sendToChannel(int user_talk, std::string msg, std::string name_chan);
		
		//=========================COMMANDS======================//
		void						cmdToConnect();
		void						passCmd();
		void						userCmd();
		void						nickCmd();
		bool						nicknameIsValid(std::string nick);
		void						privMsgCmd();
		std::string					joinMessages();
		void						joinCmd();
		void						quitCmd();
};