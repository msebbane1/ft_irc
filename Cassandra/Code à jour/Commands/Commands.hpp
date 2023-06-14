/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/14 17:33:03 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

///////INCLUDES//////
#include "../Server.hpp"
# include <sstream>

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
		
		void						create_oa_join(std::string name_chann, Commands *cmd, Server *s, Messages *m, Client *user, std::string key);
		
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
		void						partCmd();
		void						topicCmd();
		void						leaveMultiChan();
		//========================== MODE ==========================//
		void						modeCmd(); // fonction dispatch for user or channel
		void						modeOnUser(); // gestion mode on user
		void						modeOnChannel(); // gestion mode on Channgel
		//========================= MODE ON USER ==================//
		void						setInvisibleMode();// mode on user
		//========================= MODE ON CHANNEL================//
		void						setChanInviteOnlyMode(); //i
		void						setChanRestrictTopic();//t
		void						setChanKey();//k
		void						setChanOperator();//o
		void						setChanLimit();//l
		void						banUser();//b
		int							verifModeParam(); // check param
		int							countOption();//compte le nb d'optioin de mode
		char						findIndice();
		void						splitOption(); //gestion multioption
		int							ft_stoi( std::string & s ); //cast string en int
		void						printListCmd();
		int							verifUser();
};