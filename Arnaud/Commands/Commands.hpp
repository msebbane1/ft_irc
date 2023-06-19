/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/19 13:14:23 by asahonet         ###   ########.fr       */
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

		void						exec_cmd(Client *bot);

		bool						chanExist(std::string name_chan);
		
		//=========================COMMANDS======================//
		void						cmdToConnect();

		//========================= ACCESSORS ======================//

		char						getIndice();
		void						setIndice(char operand);
		
		//========================= COMMANDS ======================//
		void						passCmd();
		void						userCmd();
		void						nickCmd();
		void						privMsgCmd(Client *bot);
		void						joinCmd();
		void						quitCmd();
		void						operCmd();
		void						killCmd();
		void						inviteCmd();
		void						kickCmd();
		void						listCmd();
		void						partCmd();
		void						topicCmd();
		//void						modeCmd();

		//======================== Mode Command =====================//
		/*
		void						modeCmd(); // fonction dispatch option mode
		void						setChanInviteOnlyMode(); //i
		void						setChanRestrictTopic();//t
		void						setChanKey();//k
		void						setChanOperator();//o
		void						setChanLimit();//l
		int							verifModeParam(); // check param
		int							countOption();//compte le nb d'optioin de mode
		char						findIndice();
		void						splitOption();
		int							ft_stoi( std::string & s );
		*/
		//======================== COMMANDS UTILS ======================//
		
		std::string					joinReason();
		std::string					joinMessages();
		void						leaveMultiChan();
		void						create_oa_join(std::string name_chann, std::string key);
};