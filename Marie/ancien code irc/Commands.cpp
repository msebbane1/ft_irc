/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/23 11:23:02 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"

Commands::Commands(){}

Commands::~Commands(){}

/*--------------------------------------------------------*/

void	Commands::exec_cmd(Server *server, std::vector<std::string>	cmd, Client *user, int user_talk)
{
	std::string					msg;

	if (server->isCommandIrc(cmd[0]) == false)
	{
		std::cout << "[not a command]" << std::endl;
		return ;
	}
	//================================CMD CONNECTION REGISTER==============================//
	if (user->isConnected() == false)
		cmdToConnect(server, cmd, user, user_talk);
	if (user->isConnected() == true)
	{
	//================================CMD CONNECTION MSG==============================//
		if (cmd[0] == "PASS")
			std::cout << "Already connected." << std::endl;
		else if (cmd[0] == "USER")
			std::cout << "Already set USER." << std::endl;
		else if (cmd[0] == "NICK")
			std::cout << "Already set NICK." << std::endl;
	//================================CMD CHANNEL OPERATION==============================//
		else if (cmd[0] == "PRIVMSG")
		{
			//if (chanExist(cmd[1]) == false)
				//return ;
			privMsgCmd(user_talk, user, server, cmd);	
			return ;
		}
    	else if (cmd[0] == "JOIN")
		{
			if (chanExist(cmd[1]) == false)
			{
				std::cout << "join====" << std::endl;
				Channel	*chan = new Channel(cmd[1]);
				this->_list_chan.push_back(chan);
				chan->addUser(server->_list_client[user_talk], user_talk);
			}
			else
			{
				std::cout << "join====2222222" << std::endl;
				takeServ(cmd[1])->addUser(server->_list_client[user_talk], user_talk);
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
		server->_fd_users_dc.push_back(cl);
		
		msg = "No try left: User is disconnected.\n";
		send(cl, msg.c_str(), msg.size(), 0);
		std::cout << "Client " << cl << " has been disconnected." << std::endl;
		return (false);
	}
	else
	{
		msg = "It left you " + std::to_string(3 - user->get_pass_try()) + " try.\n";
		send(cl, msg.c_str(), msg.size(), 0);
		return (false);
	}
	return (false);
}

void Commands::cmdToConnect(Server *server, std::vector<std::string> cmd, Client *user, int user_talk)
{
	std::string		msg;
	
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

void	Commands::privMsgCmd(int user_talk, Client *user, Server *server, std::vector<std::string> cmd) // std::string name_chan
{
	std::string	msg;
	int			fd_received;

	for(std::map<int, Client *>::iterator ite = server->_list_client.begin(); ite != server->_list_client.end(); ite++)
	{
		fd_received = ite->first;
	//AJOUT nickname obligatoire avant de se connecter NICKNAME IS SET
	//std::cout << "[CMD : PRIVMSG]" << server->_list_client[fd_received]->getUser() << "/" << cmd[1] << "/";
	std::cout << "haalo2 ==== " << server->_list_client[fd_received]->getNickname() << std::endl;
	if(cmd[1] == server->_list_client[fd_received]->getNickname() && !cmd[2].empty()) // ou name of channel 
	{
		if (fd_received != user_talk && server->_list_client[fd_received]->passwordIsSet() == true) // && userIsInChan(name_chan, fd_received) == true 
		{
			msg = "Message of [ID: " + std::to_string(user_talk) + "] : " + cmd[2] + "\n";
			if (send(fd_received, msg.c_str(), msg.length(), 0) < 0)
			{
				std::cout << "Send failed" << std::endl;
				server->getFdUsersDc().push_back(user_talk);
				std::cout << "Client " << user_talk << " has been disconnected." << std::endl;
			}
		}
	}
	if(cmd[1] != server->_list_client[fd_received]->getNickname())
	{
		msg = "\x1B[37mDidn't find the nickname or name channel called : " + cmd[1] + "\033[0m\n";
		send(user_talk, msg.c_str(), msg.length(), 0);
	}
	else if(cmd[2].empty() && cmd[1] == server->_list_client[fd_received]->getNickname())
	{
		msg = "PRIVMSG <target> <text to be send> \n";
		send(user_talk, msg.c_str(), msg.length(), 0);
	}
	}
	if (user)
		server->_historic.push_back(msg);
}

/*--------------------------------------------------------*/

std::vector<std::string>	Commands::splitCustom(std::string buf, char charset)
{
	std::string					tmp;
	std::vector<std::string>	split;
	unsigned long				e_i = 0;
	unsigned long				s_i = 0;

	for (unsigned long i = 0; i <= buf.size(); i++)
	{
		if (buf[i] == charset || i == buf.size())
		{
			e_i = i;
			tmp.clear();
			tmp.append(buf, s_i, e_i - s_i);
			split.push_back(tmp);
			s_i = e_i + 1;
		}
	}
	return (split);
}

/*--------------------------------------------------------*/
bool	Commands::chanExist(std::string name)
{
	for (unsigned long i = 0; i < this->_list_chan.size(); i++)
	{
		std::cout << "==========" << this->_list_chan[i]->getName() << name << std::endl;
		if (this->_list_chan[i]->getName() == (name))
		{
			return (true);
		}
	}
	return (false);
}

bool	Commands::userIsInChan(std::string name_chan, int fd_user)
{
	int	j = 0;
	
	for (unsigned long i = 0; i < this->_list_chan.size(); i++)
	{
		if (this->_list_chan[i]->getName() == (name_chan + '\n'))
		{
			j = i;
			break ;
		}
	}
	for (std::map<int, Client*>::iterator it = this->_list_chan[j]->getListUserCo().begin(); it != this->_list_chan[j]->getListUserCo().end(); it++)
	{
		if (it->first == fd_user)
			return (true);
	}
	return (false);
}

Channel*	Commands::takeServ(std::string name)
{
	for (unsigned long i = 0; i < this->_list_chan.size(); i++)
	{
		if (this->_list_chan[i]->getName() == (name + '\n'))
			return (this->_list_chan[i]);
	}
	return (0);
}