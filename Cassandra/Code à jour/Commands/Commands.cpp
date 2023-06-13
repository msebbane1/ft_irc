/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/13 15:05:11 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

Commands::Commands(Server *s, Client *c, int fd_c, std::vector<std::string> linecmd, Messages msg): _s(s), _user(c), _fd_user(fd_c), _line_cmd(linecmd)
{
	this->_msg = &msg;
}

Commands::~Commands()
{
}

/*--------------------------------------------------------*/

void	Commands::exec_cmd()
{
	std::string	msg;

	if (this->_s->isCommandIrc(this->_line_cmd[0]) == false)
	{
		this->_msg->ERR_UNKNOWNCOMMAND(this->_line_cmd[0], this->_fd_user);
		return ;
	}
//==================================CMD CONNECTION REGISTER==============================//
	if (this->_user->isConnected() == false)
		cmdToConnect();
	else
	{
	//=======================================COMMANDS=========================================//
		if (this->_line_cmd[0] == "PASS")
			passCmd();
		else if (this->_line_cmd[0] == "USER")
			userCmd();
		else if (this->_line_cmd[0] == "NICK")
			nickCmd();
	//================================CMD CHANNEL OPERATION==============================//
		else if (this->_line_cmd[0] == "PRIVMSG")
			privMsgCmd();	
    	else if (this->_line_cmd[0] == "JOIN")
			joinCmd();
		else if (this->_line_cmd[0] == "QUIT")
			quitCmd();
		else if (this->_line_cmd[0] == "NOTICE") // <pseudonyme> <texte>
			privMsgCmd();
    	else if (this->_line_cmd[0] == "OPER")
			operCmd();
    	else if (this->_line_cmd[0] == "KILL")
			killCmd();
    	else if (this->_line_cmd[0] == "PART") //  <canal>{,< canal >}
			partCmd();
    	else if (this->_line_cmd[0] == "TOPIC") // <canal> [<sujet>]
			topicCmd();
    	else if (this->_line_cmd[0] == "MODE")
			modeCmd(); // <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]
		else if (this->_line_cmd[0] == "LIST"){} // [<canal>{,<canal>} [<serveur>]]
		else if (this->_line_cmd[0] == "INVITE"){} // <pseudonyme> <canal>
		else if (this->_line_cmd[0] == "KICK"){} // <canal> <utilisateur> [<commentaire>]
		else if (this->_line_cmd[0] == "PING") //<serveur1> [<serveur2>]
		{			
			std::string msg = ":localhost PONG :localhost\r\n";
			send(this->_fd_user, msg.c_str(), msg.size(), 0);
		}
		else if (this->_line_cmd[0] == "WHOIS")
			return ;
		else if (this->_line_cmd[0] == "WHO")
			return ;
		else if (this->_line_cmd[0] == "NAMES")
			return ;
	}
}

//-------Commands to connects------//

void	Commands::cmdToConnect()
{
	if (this->_line_cmd[0] == "PASS")
		passCmd();
	else if (_line_cmd[0] == "USER" && this->_user->passwordIsSet() == true) 
		userCmd();
	else if (_line_cmd[0] == "NICK" && this->_user->passwordIsSet() == true)
		nickCmd();
	else if(_line_cmd[0] != "CAP")
		this->_msg->ERR_NOTREGISTERED(this->_fd_user);

	if (this->_user->isConnected())
		this->_msg->welcome(this->_user, this->_fd_user);
}

/*-------------------------CHANNELS-------------------------------*/

bool	Commands::chanExist(std::string name_chan)
{
	std::map<std::string, Channel*> map = this->_s->getListChan();
	std::map<std::string, Channel*>::iterator it = map.begin();
	
	if (!map.empty())
	{
		while (it != map.end())
		{
			if (it->first == name_chan)
				return (true);
			it++;
		}
	}
	return (false);
}

//=========================ACCESSORS======================//

char	Commands::getIndice(){
	return this->_indice;
}

void	Commands::setIndice(char operand){
	this->_indice = operand;
}
