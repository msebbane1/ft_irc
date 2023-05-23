/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/23 18:24:46 by msebbane         ###   ########.fr       */
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
	
	//this->_s->setListClient(this->_fd_user, this->_user);
	if (this->_s->isCommandIrc(this->_line_cmd[0]) == false)
	{
		std::cout << "[not a command]" << std::endl;
		return ;
	}
	//================================CMD CONNECTION REGISTER==============================//
	if (this->_user->isConnected() == false)
		cmdToConnect(this->_s, this->_line_cmd, this->_user, this->_fd_user);
	else
	{
	//================================CMD CONNECTION MSG==============================//
		if (this->_line_cmd[0] == "PASS")
		{
			std::cout << "Already connected." << std::endl;
			return;
		}
		else if (this->_line_cmd[0] == "USER")
		{
			std::cout << "Already set USER." << std::endl;
			return ;
		}
		else if (this->_line_cmd[0] == "NICK")
		{
			std::cout << "Already set NICK." << std::endl;
			return ;
		}
	//================================CMD CHANNEL OPERATION==============================//
		else if (this->_line_cmd[0] == "PRIVMSG")
		{
			//if (chanExist(cmd[1]) == false)
				//return ;
			privMsgCmd(this->_fd_user, this->_line_cmd, this->_user);	
			return ;
		}
    	else if (this->_line_cmd[0] == "JOIN")
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
	}
}

/*--------------------------------------------------------*/

bool	Commands::passCmd(std::vector<std::string> line, int cl, Client *user, Server *server)
{
	std::string		msg;
	
	if (line[1] == server->getPassword())
	{
		user->setPassword();
		msg = "====== Good password! ======\n";
		send(cl, msg.c_str(), msg.size(), 0);
		return (true);
	}
	user->increment_pass_try();
	if (user->get_pass_try() == 3)
	{
		server->setFdUsersDc(cl);
		msg = "No try left: User is disconnected.\n";
		send(cl, msg.c_str(), msg.size(), 0);
		std::cout << "Client " << cl << " has been disconnected." << std::endl;
	}
	else
	{
		msg = "It left you " + std::to_string(3 - user->get_pass_try()) + " try.\n";
		send(cl, msg.c_str(), msg.size(), 0);
	}
	return (false);
}

void	Commands::cmdToConnect(Server *server, std::vector<std::string> cmd, Client *user, int user_talk)
{
	std::string	msg;

	if (cmd[0] == "PASS" && !user->passwordIsSet())
	{
		if (passCmd(cmd, user_talk, user, server) == false)
		{
			std::cout << "[CMD : PASS]" << std::endl;
			msg = "You entered a wrong password.\n";
			send(user_talk, msg.c_str(), msg.size(), 0);
			return ;
		}
	}
	else if (cmd[0] == "USER" && user->passwordIsSet() == true)
	{
		//verifier user valide PARSE USER // ne doit pas etre vide // PARSE COMMAND USER 3 ARGUMENTS
		std::cout << "[CMD : USER]" << std::endl;
		user->setUser(cmd[1]);
		msg = "====== USER is set ===== :" + user->getUser() + "\n";
		send(user_talk, msg.c_str(), msg.size(), 0); // faire une fonction send ??
	}
	else if (cmd[0] == "NICK" && user->passwordIsSet() == true)
	{
			//verifier nickname valide PARSE NICK
		std::cout << "[CMD : NICK]" << std::endl;
		user->setNickname(cmd[1]);
		msg = "======nickname is set=====:" + user->getNickname() + "\n";
		send(user_talk, msg.c_str(), msg.size(), 0);
		std::cout << "======nickname is set=====" << std::endl;
	}
}

void	Commands::privMsgCmd(int user_talk, std::vector<std::string> cmd, Client *user)
{
	std::string msg;

	if(cmd.size() <= 2)
	{
		//msg = "PRIVMSG <target> <text to be send> \n";
		//send(user_talk, msg.c_str(), msg.length(), 0);
		this->_s->errorSend("411", this->_user->getNickname(), "No receiver given", this->_user->get_fd());
	}
	else if(cmd.size() > 2)
	{
		/* POUR CHANNEL
		if (cmd[1][0] == '#')
			cmd[1].erase(0, 1);
		if(chanExist(cmd[1]) == true)
		{
			ENVOIE MSG DIFFERENT
		}
		*/
		if(this->_s->clientExist(cmd[1]) == true)
		{
			msg = "Message of [ID: " + std::to_string(user_talk) + "] : NICK :" + user->getNickname() + ": " + cmd[2] + "\n";
			std::cout << "=======" << this->_s->getClient(cmd[1])->get_fd() << std::endl;
			send(this->_s->getClient(cmd[1])->get_fd(), msg.c_str(), msg.length(), 0);
		}
		else
		{
			msg = "\x1B[37mDidn't find the nickname or name channel called : " + cmd[1] + "\033[0m\n";
			send(user_talk, msg.c_str(), msg.length(), 0);
		}
	}
}

/*--------------------------------------------------------*/
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
		if ( this->_s->getListChan()[i]->getName() == (name_chan + '\n'))
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

Channel*	Commands::takeServ(std::string name)
{
	for (unsigned long i = 0; i < this->_s->getListChan().size(); i++)
	{
		if (this->_s->getListChan()[i]->getName() == (name + '\n'))
			return (this->_s->getListChan()[i]);
	}
	return (0);
}
