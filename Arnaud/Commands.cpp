/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/23 12:09:54 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"

Commands::Commands(Server *s, Client *c, int fd_c, std::vector<std::string> linecmd): _s(s), _user(c), _fd_user(fd_c), _line_cmd(linecmd)
{
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
			std::cout << "Already connected." << std::endl;
		else if (this->_line_cmd[0] == "USER")
			std::cout << "Already set USER." << std::endl;
		else if (this->_line_cmd[0] == "NICK")
			std::cout << "Already set NICK." << std::endl;
	//================================CMD CHANNEL OPERATION==============================//
		else if (this->_line_cmd[0] == "PRIVMSG")
		{
			//if (chanExist(cmd[1]) == false)
				//return ;
			privMsgCmd(this->_fd_user, this->_line_cmd);	
			return ;
		}
    	else if (this->_line_cmd[0] == "JOIN")
		{
			if (chanExist(this->_line_cmd[1]) == false)
			{
				std::cout << "join====" << std::endl;
				Channel	*chan = new Channel(this->_line_cmd[1]);
				this->_s->addListChan(chan);
				chan->addUser(this->_s->getListClient()[this->_fd_user], this->_fd_user);
			}
			else
			{
				std::cout << "join====2222222" << std::endl;
				takeServ(this->_line_cmd[1])->addUser(this->_s->getListClient()[this->_fd_user], this->_fd_user);
			}
		}
	}
}

/*--------------------------------------------------------*/

bool	Commands::passCmd(std::vector<std::string> line, int cl, Client *user, Server *server)
{
	std::string		msg;
	
	//if (line[1].find("\n") > 0)
		//line[1].erase(line[1].size() - 1);
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
		//verifier user valide PARSE USER
		std::cout << "[CMD : USER]" << std::endl;
		user->setUser(cmd[1]);
		msg = "====== USER is set ===== :" + user->getUser() + "\n";
		send(user_talk, msg.c_str(), msg.size(), 0);
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

/*--------------------------------------------------------*/

void	Commands::privMsgCmd(int user_talk, std::vector<std::string> cmd) // std::string name_chan
{
	std::string	msg;
	int			fd_received;
	std::string	dest = cmd[1];

	for(std::map<int, Client *>::iterator ite = this->_s->getListClient().begin(); ite != this->_s->getListClient().end(); ite++)
	{
		fd_received = ite->first;
		//AJOUT nickname obligatoire avant de se connecter NICKNAME IS SET
		//std::cout << "[CMD : PRIVMSG]" << server->_list_client[fd_received]->getUser() << "/" << cmd[1] << "/";
		if (dest.find('#') == 0 || dest.find('&') == 0)
		{
			if (fd_received != user_talk && userIsInChan(dest, fd_received) == true && this->_s->getListClient()[fd_received]->passwordIsSet() == true)
			{
				msg = dest + " [ID: " + std::to_string(user_talk) + "] : " + cmd[2] + "\n";
				if (send(fd_received, msg.c_str(), msg.length(), 0) < 0)
				{
					std::cout << "Send failed" << std::endl;
					this->_s->setFdUsersDc(user_talk);
					std::cout << "Client " << user_talk << " has been disconnected." << std::endl;
				}
			}
		}
		if(dest == this->_s->getListClient()[fd_received]->getNickname() && !cmd[2].empty()) // ou name of channel 
		{
			if (fd_received != user_talk && this->_s->getListClient()[fd_received]->passwordIsSet() == true) // && userIsInChan(name_chan, fd_received) == true 
			{
				msg = dest + " [ID: " + std::to_string(user_talk) + "] : " + cmd[2] + "\n";
				if (send(fd_received, msg.c_str(), msg.length(), 0) < 0)
				{
					std::cout << "Send failed" << std::endl;
					this->_s->setFdUsersDc(user_talk);
					std::cout << "Client " << user_talk << " has been disconnected." << std::endl;
				}
			}
		}
		else if(dest != this->_s->getListClient()[fd_received]->getNickname())
		{
			msg = "\x1B[37mDidn't find the nickname or name channel called : " + dest + "\033[0m\n";
			send(user_talk, msg.c_str(), msg.length(), 0);
		}
		else if(cmd[2].empty() && dest == this->_s->getListClient()[fd_received]->getNickname())
		{
			msg = "PRIVMSG <target> <text to be send> \n";
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

Channel*	Commands::takeServ(std::string name)
{
	for (unsigned long i = 0; i < this->_s->getListChan().size(); i++)
	{
		if (this->_s->getListChan()[i]->getName() == (name + '\n'))
			return (this->_s->getListChan()[i]);
	}
	return (0);
}