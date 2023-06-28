/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:46 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/22 07:33:45 by msebbane         ###   ########.fr       */
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
		Client*						_bot;
		int							_fd_user;
		std::vector<std::string>	_line_cmd;
		char						_indice;
		std::vector<char>			_optionList;// gestion d'option multiple

	public :
		Commands(Server *s, Client *c, int fd_c, std::vector<std::string> linecmd, Messages msg);
		~Commands();

		void						exec_cmd();
		bool						chanExist(std::string name_chan);
		void						cmdToConnect();
		
		//========================= ACCESSORS ======================//

		char						getIndice();
		void						setIndice(char operand);
		
		//========================= COMMANDS ======================//
		void						passCmd();
		void						userCmd();
		void						nickCmd();
		void						privMsgCmd();
		void						joinCmd();
		void						quitCmd();
		void						operCmd();
		void						killCmd();
		void						inviteCmd();
		void						kickCmd();
		void						listCmd();
		void						partCmd();
		void						topicCmd();

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
		
		//======================== COMMANDS UTILS ======================//
		Channel *					rplToAllChan();
		std::string					joinMessages(int line);
		void						leaveMultiChan();
		void						create_oa_join(std::string name_chann, std::string key);
		bool						userOnChannel();

		void 						setBot();
};