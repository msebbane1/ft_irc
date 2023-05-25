/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/25 19:26:06 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"

Commands::Commands(Server *s, Client *c, int fd_c, std::vector<std::string> linecmd): _s(s), _user(c), _fd_user(fd_c), _line_cmd(linecmd){}

Commands::~Commands(){}

/*--------------------------------------------------------*/

void	Commands::exec_cmd()
{
	std::string	msg;

	if (this->_s->isCommandIrc(this->_line_cmd[0]) == false)
		return ;
	//================================CMD CONNECTION REGISTER==============================//
	if (this->_user->isConnected() == false)
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
		else if (this->_line_cmd[0] == "AUTHENTICATE"){}
		else if (this->_line_cmd[0] == "QUIT"){} //  [<Message de départ >]
    	else if (this->_line_cmd[0] == "PART"){} //  <canal>{,< canal >}
	  	else if (this->_line_cmd[0] == "NOTICE"){} // <pseudonyme> <texte>
    	else if (this->_line_cmd[0] == "TOPIC"){} // <canal> [<sujet>]
    	else if (this->_line_cmd[0] == "MODE"){ //{} // <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]
			std::cout << "else if MODE" << std::endl;
			modeCmd();
		}
		else if (this->_line_cmd[0] == "LIST"){} // [<canal>{,<canal>} [<serveur>]]
    	else if (this->_line_cmd[0] == "OPER"){} //params : <utilisateur> <mot de passe>
    	else if (this->_line_cmd[0] == "KILL"){} // <pseudonyme> <commentaire>
    	else if (this->_line_cmd[0] == "PING"){} // <serveur1> [<serveur2>]
		else if (this->_line_cmd[0] == "INVITE"){} // <pseudonyme> <canal>
		else if (this->_line_cmd[0] == "KICK"){} // <canal> <utilisateur> [<commentaire>]
		else if (this->_line_cmd[0] == "INFO"){}// [<serveur>]
	}
}

/*--------------------------------------------------COMMANDS---------------------------------------------*/

//-------Commands to connects------//

void	Commands::cmdToConnect()
{
	std::string	msg;

	if (this->_line_cmd[0] == "PASS")
		passCmd();
	else if (_line_cmd[0] == "USER" && this->_user->passwordIsSet() == true) // Paramètres: <nom d'utilisateur> <hôte> <nom de serveur> <nom réel>
		userCmd();
	else if (_line_cmd[0] == "NICK" && this->_user->passwordIsSet() == true)
		nickCmd();
	else if(!this->_user->isConnected())
		std::cout << "send error" << std::endl;
	if (this->_user->isConnected())
		this->_s->welcomeMsg(this->_user->getUser(), this->_user->getNickname(), this->_fd_user);
}

//---------------------PASS-------------------//

void	Commands::passCmd()
{
	std::string		msg;
	
	
	if (!this->_user->passwordIsSet())
	{
		if (this->_line_cmd.size() < 2)
			std::cout << "send error" << std::endl;
		else if (this->_line_cmd[1] == this->_s->getPassword())
		{
			this->_user->setPassword();
			msg = "====== Good password! ======\n";
			send(this->_fd_user, msg.c_str(), msg.size(), 0);
		}
		else
		{
			this->_user->increment_pass_try();
			if (this->_user->get_pass_try() == 3)
			{
				this->_s->setFdUsersDc(this->_fd_user);
				msg = "No try left: User is disconnected.\n";
				send(this->_fd_user, msg.c_str(), msg.size(), 0);
				std::cout << "Client " << this->_fd_user << " has been disconnected." << std::endl;
			}
			else
			{
				msg = "Wrong password.It left you " + std::to_string(3 - this->_user->get_pass_try()) + " try.\n";
				send(this->_fd_user, msg.c_str(), msg.size(), 0);
			}
		}
	}
	if (_line_cmd[0] == "PASS" && this->_user->passwordIsSet())
		std::cout << "Already set PASS." << std::endl;
}

//---------------------USER-------------------//

void	Commands::userCmd()
{
	std::string msg;
	
	if(!this->_user->realnameIsSet())
	{
		if (this->_line_cmd.size() < 5)
			std::cout << "send error" << std::endl;
		if (this->_line_cmd[4][0] == ':' && this->_line_cmd[4].size() >= 2)
		{
			_line_cmd[4] = _line_cmd[4].substr(1, _line_cmd[4].size() - 1);
			std::string realname = _line_cmd[4] + " " + _line_cmd[5];
			this->_user->setRealname(realname);
			this->_user->setUser(this->_line_cmd[1]);
			msg = "====== Realname is set ===== :" + this->_user->getRealname() + "\n";
			send(this->_fd_user, msg.c_str(), msg.size(), 0);
			msg = "====== USER is set ===== :" + this->_user->getUser() + "\n";
			send(this->_fd_user, msg.c_str(), msg.size(), 0);
		}
		else if(this->_line_cmd[4][0] != ':')
		{
			msg = "USER <nom d'utilisateur> <hôte> <nom de serveur> <nom réel> \n";
			send(this->_fd_user, msg.c_str(), msg.size(), 0);
		}
	}
	else if(_line_cmd[0] == "USER" && this->_user->realnameIsSet())
		std::cout << "Already set USER." << std::endl;
		
}

//---------------------NICK-------------------//

bool	Commands::nicknameIsValid(std::string nick)
{
	for (int i = 0; nick[i]; i++)
	{
		if(isalnum(nick[i]))
		{
			if(isdigit(nick[i]))
				return false;
			else
				return true;
		}
	}
	return false;
}

void	Commands::nickCmd()
{
	std::string	msg;

	if(!this->_user->nicknameIsSet())
	{
		if (_line_cmd.size() == 1 ) 
		{
			this->_s->errorSend("431", this->_user->getNickname(), "No nickname given", this->_user->get_fd() );
			return ;
		}
		if (nicknameIsValid(this->_line_cmd[1]) == true)
		{
			this->_user->setNickname(_line_cmd[1]);
			msg = "======nickname is set=====:" + this->_user->getNickname() + "\n";
			send(this->_fd_user, msg.c_str(), msg.size(), 0);
			std::cout << "======nickname is set=====" << std::endl;
		}
		else
			std::cout << "nickname not valid" << std::endl;
	}
	else if(_line_cmd[0] == "NICK" && this->_user->nicknameIsSet())
	{
		//Utilisateur peux changer de nickname
		std::cout << "Already set Nick." << std::endl;
	}
}

//---------------------PRIVMSG-------------------//

void	Commands::privMsgCmd()
{
	std::string	msg;

	if(this->_line_cmd.size() <= 2)
		this->_s->errorSend("411", this->_user->getNickname(), "No receiver given", this->_user->get_fd()); // a regarder la doc
	else if(this->_line_cmd.size() > 2)
	{
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
		else if(this->_s->clientExist(this->_line_cmd[1]) == true)
		{
			msg = this->_user->getNickname() + " [ID: " + std::to_string(this->_fd_user) + "] : " + this->_line_cmd[2] + "\n";
			std::cout << "=======" << this->_s->getClient(this->_line_cmd[1])->get_fd() << std::endl;
			send(this->_s->getClient(this->_line_cmd[1])->get_fd(), msg.c_str(), msg.length(), 0);
		}
		else
		{
			msg = "\x1B[37mDidn't find the nickname or name channel called : " + this->_line_cmd[1] + "\033[0m\n";
			send(this->_fd_user, msg.c_str(), msg.length(), 0);
		}
	}
}


//---------------------JOIN-------------------//

void	Commands::joinCmd()
{
	if (chanExist(this->_line_cmd[1]) == false)
	{
		Channel	*chan = new Channel(this->_line_cmd[1]);
		this->_s->addListChan(chan);
		chan->addUser(this->_s->getListClient()[this->_fd_user], this->_fd_user);
		}
	else
	{
		takeServ(this->_line_cmd[1])->addUser(this->_s->getListClient()[this->_fd_user], this->_fd_user);
	}
}






/*-------------------------CHANNELS-------------------------------*/
bool	Commands::chanExist(std::string name)
{
	for (unsigned long i = 0; i < this->_s->getListChan().size(); i++)
	{
		std::cout << "==========" << this->_s->getListChan()[i]->getName() << name << std::endl;
		if (this->_s->getListChan()[i]->getName() == (name))
		{
			return (true);
		}
	}
	return (false);
}

bool	Commands::userIsInChan(std::string name_chan, int fd_user)
{
	int	j = 0;
	
	for (unsigned long i = 0; i < this->_s->getListChan().size(); i++)
	{
		if (this->_s->getListChan()[i]->getName() == (name_chan + '\n') || this->_s->getListChan()[i]->getName() == name_chan)
		{
			j = i;
			break ;
		}
	}
	for (std::map<int, Client*>::iterator it = this->_s->getListChan()[j]->getListUserCo().begin(); it != this->_s->getListChan()[j]->getListUserCo().end(); it++)
	{
		if (it->first == fd_user)
			return (true);
	}
	return (false);
}

void	Commands::sendToChannel(int user_talk, std::string msg, std::string chan)
{
	int	j = 0;
	
	for (unsigned long i = 0; i < this->_s->getListChan().size(); i++)
	{
		if (this->_s->getListChan()[i]->getName() == chan)
		{
			j = i;
			break ;
		}
	}
	for (std::map<int, Client*>::iterator it = this->_s->getListChan()[j]->getListUserCo().begin(); it != this->_s->getListChan()[j]->getListUserCo().end(); it++)
	{
		if (it->first != user_talk)
		{
			send(it->first, msg.c_str(), msg.size(), 0);
		}
	}
}

Channel*	Commands::takeServ(std::string name)
{
	for (unsigned long i = 0; i < this->_s->getListChan().size(); i++)
	{
		if (this->_s->getListChan()[i]->getName() == (name + '\n'))
			return (this->_s->getListChan()[i]);
	}
	return (0);
}

/*--------------------- MODE ----------------------*/
//change the channels mode : parametre attendu : /mode <channel> <+ | -> <mode> [parametres]
//												 cmd / name_chan / indicateur / option / optionnel(username for operator, etc)
// id a gerer: i, t, k, o, l; + to do the action, - to undo
// i : Set/remove Invite-only channel(no param needed)
// t : Set/remove the restrictions of the TOPIC command to channel operators(topic protection, seuls les ChannelOperator peuvent changer le topic.)(no param needed)
// k : Set/remove the channel key (password) (param needed) /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
// o : Give/take channel operator privilege (param needed) /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
// l : Set/remove the user limit to channel (param needed) /mode #cool +l 20 (limite le channel #cool à 20 utilisateurs)
// gerer les multiple mode ? ex: /mode #chan +tns(channel en ;ode +t, +n, +s)
void	Commands::modeCmd(){

	//verifier le nombre de param (entre 4 et 5)
	//std::string msg =  std::to_string(_line_cmd.size());
	//send(_s->getFdServer(), msg.c_str(), msg.length(), 0);
	if( _line_cmd.size() < 4)
		std::cerr << "=== 461 === ERR_NEEDMOREPARAMS ====" << std::endl;
	//faire un switch pour chaque option (461 ERR_NEEDMOREPARAMS)
	//verifier aussi l'indicateur ( + ou - )
	//repartition
}
// Message d'erreur + code d'erreur
// ERR_NEEDMOREPARAMS 461               RPL_CHANNELMODEIS 324
// ERR_CHANOPRIVSNEEDED 482             ERR_NOSUCHNICK 401
// ERR_NOTONCHANNEL 442                 ERR_KEYSET 467
// RPL_BANLIST 367                      RPL_ENDOFBANLIST 368
// ERR_UNKNOWNMODE 472                  ERR_NOSUCHCHANNEL 403

// ERR_USERSDONTMATCH 502               RPL_UMODEIS 221
// 501 ERR_UMODUUNKNOWNFLAG : Cette erreur est renvoyée quand vous envoyez une commande MODE sur un utilisateur avec un mode inconnu.
// Faut-il gerer la version de MODE sur Utilisateur ? +i, +o