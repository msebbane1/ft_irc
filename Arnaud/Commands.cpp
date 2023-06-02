/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/02 10:23:21 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"

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
	//================================CMD CONNECTION REGISTER==============================//
	
	if (this->_line_cmd[0] == "CAP")
	{
		if (this->_line_cmd[0] == "PASS")
			passCmd();
		else if (_line_cmd[0] == "USER" && this->_user->passwordIsSet() == true) 
			userCmd();
		else if (_line_cmd[0] == "NICK" && this->_user->passwordIsSet() == true)
			nickCmd();
		if (this->_user->isConnected())
			this->_msg->welcome(this->_user, this->_fd_user);
	}
	else if (this->_user->isConnected() == false)
		cmdToConnect();
	else
	{
	//================================CMD CONNECTION MSG==============================//
		if (this->_line_cmd[0] == "PASS")
			passCmd();
		else if (this->_line_cmd[0] == "USER")
			userCmd();
		else if (this->_line_cmd[0] == "NICK")
			nickCmd();
	//================================CMD CHANNEL OPERATION==============================//
		else if (this->_line_cmd[0] == "PRIVMSG")
			privMsgCmd();	
    	else if (this->_line_cmd[0] == "JOIN") // <canal>{,<canal>} [<clé>{,<clé>}]
			joinCmd();
		else if (this->_line_cmd[0] == "QUIT") //  [<Message de départ >]
			quitCmd();
		else if (this->_line_cmd[0] == "PING") //<serveur1> [<serveur2>]
		{			
			std::string msg = ":localhost PONG :localhost\r\n";
			send(this->_fd_user, msg.c_str(), msg.size(), 0);
		}
		else if (this->_line_cmd[0] == "WHOIS")
			return ;
		else if (this->_line_cmd[0] == "INFO"){}// [<serveur>]
		else if (this->_line_cmd[0] == "AUTHENTICATE"){}
    	else if (this->_line_cmd[0] == "PART"){} //  <canal>{,< canal >}
	  	else if (this->_line_cmd[0] == "NOTICE"){} // <pseudonyme> <texte>
    	else if (this->_line_cmd[0] == "TOPIC"){} // <canal> [<sujet>]
    	else if (this->_line_cmd[0] == "MODE"){} // <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]
		else if (this->_line_cmd[0] == "LIST"){} // [<canal>{,<canal>} [<serveur>]]
    	else if (this->_line_cmd[0] == "OPER"){} //params : <utilisateur> <mot de passe>
    	else if (this->_line_cmd[0] == "KILL"){} // <pseudonyme> <commentaire>
		else if (this->_line_cmd[0] == "INVITE"){} // <pseudonyme> <canal>
		else if (this->_line_cmd[0] == "KICK"){} // <canal> <utilisateur> [<commentaire>]
	}
}

/*--------------------------------------------------COMMANDS---------------------------------------------*/

//-------Commands to connects------//

void	Commands::cmdToConnect()
{
	std::string	msg;

	if (this->_line_cmd[0] == "PASS")
		passCmd();
	else if (_line_cmd[0] == "USER" && this->_user->passwordIsSet() == true)
		userCmd();
	else if (_line_cmd[0] == "NICK" && this->_user->passwordIsSet() == true)
		nickCmd();
	else
		this->_msg->ERR_NOTREGISTERED(this->_fd_user);
	if (this->_user->isConnected())
		this->_msg->welcome(this->_user, this->_fd_user);
}

//---------------------PASS-------------------//

void	Commands::passCmd()
{
	std::string		msg;
	
	if (!this->_user->passwordIsSet())
	{
		if (this->_line_cmd.size() < 2)
			this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		else if (this->_line_cmd[1] == this->_s->getPassword())
		{
			this->_user->setPassword();
			std::cout << "====== CONNECTED! ======\n";
		}
		else
		{
			this->_msg->ERR_PASSWDMISMATCH(this->_fd_user);
			this->_user->increment_pass_try();
			if (this->_user->get_pass_try() == 3)
				this->_s->setFdUsersDc(this->_fd_user);
		}
	}
	else if (this->_line_cmd[0] == "PASS" && this->_user->passwordIsSet())
		this->_msg->ERR_ALREADYREGISTRED(this->_fd_user);
}

//---------------------USER-------------------//

void	Commands::userCmd()
{
	std::string realname;
	
	if(!this->_user->realnameIsSet())
	{
		if (this->_line_cmd.size() < 5 || this->_line_cmd[4][0] != ':')
		{
			this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
			return;
		}
		if (this->_line_cmd[4][0] == ':' && this->_line_cmd[4].size() >= 2)
		{
			this->_line_cmd[4] = this->_line_cmd[4].substr(1, this->_line_cmd[4].size() - 1);
			std::string realname = this->_line_cmd[4] + (this->_line_cmd[5].empty() ? "\0" : " " + this->_line_cmd[5]);
			this->_user->setRealname(realname);
			this->_user->setUser(this->_line_cmd[1]);
			std::cout << "====== Realname is set ===== :" << this->_user->getRealname() << "\n";
			std::cout << "====== USER is set ===== :" << this->_user->getUser() << "\n";
		}
	}
	else if(_line_cmd[0] == "USER" && this->_user->realnameIsSet())
		this->_msg->ERR_ALREADYREGISTRED(this->_fd_user);		
}

//---------------------NICK-------------------//

bool	Commands::nicknameIsValid(std::string nick)
{
	for (int i = 0; nick[i]; i++)
	{
		if (i == 0 && (nick[i] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&'))
			return (false);
		if (nick[i] == ' ' || nick[0] == ',' || nick[0] == '?' || nick[0] == '!' 
			|| nick[0] == '@' || nick[0] == '*' || nick[0] == '.')
			return (false);
	}
	return (true);
}

void	Commands::nickCmd()
{
	std::string	msg;

	if (this->_line_cmd.size() == 1 ) 
		this->_msg->ERR_NONICKNAMEGIVEN(this->_fd_user);
	else if(this->_line_cmd.size() >= 2)
	{
		if (this->_s->clientExist(this->_line_cmd[1]))
			this->_msg->ERR_NICKNAMEINUSE(this->_line_cmd[1], this->_fd_user);
		else if (nicknameIsValid(this->_line_cmd[1]) == true)
		{
			if(this->_user->nicknameIsSet())
			{
				std::string	msg = ":" +  _user->getNickname() + "!" + _user->getUser() + "@localhost NICK :" + this->_line_cmd[1] + "\r\n"; // POUR CHANGER NICK
				if(send(this->_fd_user, msg.c_str(), msg.length(), 0) < 0)
					_msg->errorMsg("failed send");
			}
			this->_user->setNickname(this->_line_cmd[1]);
			std::cout << "======nickname is set=====: " << this->_user->getNickname() << std::endl;
		}
		else
			this->_msg->ERR_ERRONEUSNICKNAME(this->_line_cmd[1], this->_fd_user);
	}
}

//---------------------PRIVMSG-------------------//

std::string	Commands::joinMessages()
{
	std::string msg;
	std::vector<std::string>::iterator it = this->_line_cmd.begin() + 2;
	while (it != this->_line_cmd.end())
	{
		msg += *it;
		msg += " ";
		it++;
	}
	//std::cout << msg << std::endl;
	return(msg);
}

void	Commands::privMsgCmd()
{
	std::string	msg;

	if (this->_line_cmd.size() <= 2)
		this->_msg->ERR_NORECIPIENT(this->_fd_user); 
	else if (this->_line_cmd.size() > 2)
	{
		if (this->_line_cmd[1].empty())
			this->_msg->ERR_NOTEXTTOSEND(this->_fd_user);
		if (this->_line_cmd[1][0] == '#' || this->_line_cmd[1][0] == '&')
		{
			if(chanExist(this->_line_cmd[1]) == true)
			{
				msg = "\"" + this->_line_cmd[1] + "\" [" + this->_user->getNickname() + "] [ID: " + std::to_string(this->_fd_user) + "] : " + this->_line_cmd[2] + "\n";
				sendToChannel(this->_fd_user, msg, this->_line_cmd[1]);
			}
			else
			{
				msg = "\x1B[37mChannel doesn't exist, please create it if you want to communicate on it.\n";
				send(this->_fd_user, msg.c_str(), msg.length(), 0);
			}
		}
		else if (this->_s->clientExist(this->_line_cmd[1]) == true)
		{
			msg = ":" + this->_user->getNickname() + " PRIVMSG " +  this->_line_cmd[1] + " " + joinMessages() + "\r\n";
			std::cout << "=======" << this->_s->getClient(this->_line_cmd[1])->get_fd() << std::endl;
			send(this->_s->getClient(this->_line_cmd[1])->get_fd(), msg.c_str(), msg.length(), 0);
		}
		else
			this->_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
	}
}

//---------------------QUIT-------------------//

void	Commands::quitCmd() // QUIT <msg>
{
	//if (this->_line_cmd.size > 2) 
		//afficher msg dans tous les channels + msg
	this->_s->setFdUsersDc(this->_fd_user);
	if(this->_user->nicknameIsSet() == false)
		std::cout << "Client " << this->_fd_user << " has been disconnected." << std::endl;
	else
		std::cout << this->_user->getNickname() << " has been disconnected." << std::endl;
}

//---------------------JOIN-------------------//

void	Commands::joinCmd()
{
	std::string	msg;

	if (this->_line_cmd.size() == 1)
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user); //461
	else if (this->_line_cmd[1][0] != '#' || this->_line_cmd[1][0] != '&')
	{
		// A changer type erreur
		this->_msg->errorSend("", this->_user->getNickname(), "Wrong format", this->_user->get_fd());
		return;
	}
	if (chanExist(this->_line_cmd[1]) == false)
	{
		Channel	*chan = new Channel(this->_line_cmd[1], this->_user);
		this->_s->addListChan(chan);
	}
	else
	{
		this->_s->getChannel(this->_line_cmd[1])->addUser(this->_user, this->_fd_user);
	}
	msg = "You have joined successfully " + this->_line_cmd[1] + " !\n";
	send(this->_user->get_fd(), msg.c_str(), msg.size(), 0);
	
	// affichage tt user dans le chann
	std::cout << "----------------" << std::endl;
	this->_s->getChannel(this->_line_cmd[1])->displayUsers();
	std::cout << "----------------" << std::endl;
	this->_s->getChannel(this->_line_cmd[1])->displayOp();
}

/*-------------------------CHANNELS-------------------------------*/

bool	Commands::chanExist(std::string name_chan)
{
	std::map<std::string, Channel*> map = this->_s->getListChan();
	std::map<std::string, Channel*>::iterator it = map.begin();
	
	if (!map.empty())
	{
		std::cout << "chann: |" << it->first << "|" << std::endl;
		std::cout << "chann: |" << name_chan << "|" << std::endl;
		while (it != map.end())
		{
			if (it->first == name_chan)
				return (true);
			it++;
		}
	}
	return (false);
}

bool	Commands::userIsInChan(std::string name_chan, int fd_user)
{
	std::map<int, Client*> map = this->_s->getChannel(name_chan)->getListUserCo();
	
	for (std::map<int, Client*>::iterator it = map.begin(); it != map.end(); it++)
	{
		if (it->first == fd_user)
			return (true);
	}
	return (false);
}

void	Commands::sendToChannel(int user_talk, std::string msg, std::string name_chan)
{
	std::map<int, Client*> map = this->_s->getChannel(name_chan)->getListUserCo();

	for (std::map<int, Client*>::iterator it = map.begin(); it != map.end(); it++)
	{
		if (it->first != user_talk)
			send(it->first, msg.c_str(), msg.size(), 0);
	}
}

