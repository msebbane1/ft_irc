/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/06 14:07:10 by clecat           ###   ########.fr       */
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
		char						_indice;
		std::vector<char>			_optionList;// gestion d'option multiple

	public :
		Commands(Server *s, Client *c, int fd_c, std::vector<std::string> linecmd, Messages msg);
		~Commands();

		void						exec_cmd();

		bool						chanExist(std::string name_chan);
		void						sendToChannel(int user_talk, std::string msg, std::string name_chan);
		
		//=========================ACCESSORS======================//

		char						getIndice();
		void						setIndice(char operand);

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
				//====== Mode =====//
		void						modeCmd(); // fonction dispatch option mode
		void						setChanInviteOnlyMode(); //i
		void						setChanRestrictTopic();//t
		void						setChanKey();//k
		void						setChanOperator();//o
		void						setChanLimit();//l
		int							verifModeParam(); // check param
		int							countOption();//compte le nb d'optioin de mode
		char						findIndice();
		std::vector<char>			splitOption();
};