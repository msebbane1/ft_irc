/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/26 18:35:34 by clecat           ###   ########.fr       */
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
		bool						nicknameIsValid(std::string nick);
		void						privMsgCmd();
		void						joinCmd();
		//========================== MODE =========================//
		void						modeCmd(); // fonction dispatch option mode
		void						setChanInviteOnlyMode(); //i
		void						setChanRestrictTopic();//t
		void						setChanKey();//k
		void						setChanOperator();//o
		void						setChanLimit();//l
		int							verifModeParam(); // check param
};