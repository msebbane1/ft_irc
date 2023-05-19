/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/19 15:32:39 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"

Commands::Commands(){}

Commands::~Commands(){}

void	Commands::exec_cmd(Server *server, std::string buf, Client *user, int user_talk)
{
	std::vector<std::string>	cmd = splitCustom(buf, ' ');
	std::string					msg;

	if (server->isCommandIrc(cmd[0]) == false)
	{
		std::cout << "[not a command]" << std::endl;
		return ;
	}
	if (user->isConnected() == false)
	{
		if (cmd[0] == "PASS" && passCmd(cmd, user_talk, user, server) == false)
		{
			std::cout << "[CMD : PASS]" << std::endl;
			msg = "You entered a wrong password.\n";
			send(user_talk, msg.c_str(), msg.size(), 0);
		}
		else if (cmd[0] == "USER" && user->passwordIsSet() == true)
		{
			std::cout << "[CMD : USER]" << std::endl;
			user->setUser(cmd[1]);
			msg = "====== USER is set =====\n";
			send(user_talk, msg.c_str(), msg.size(), 0);
			std::cout << "=====user is set====" << std::endl;
		}
	}
	if(user->isConnected() == true)
	{
		if (cmd[0] == "NICK")
		{
			//verifier nickname valide
			std::cout << "[CMD : NICK]" << std::endl;
			user->setNickname(cmd[1]);
			msg = "======nickname is set=====\n";
			send(user_talk, msg.c_str(), msg.size(), 0);
			std::cout << "======nickname is set=====" << std::endl;
		}
		else if(cmd[0] == "PRIVMSG")
		{
			privMsgCmd(buf, user_talk, user, server);
		}
		/*
    	else if (cmd[0] == "JOIN")
        //join_cmd(cmd, user, server);
		else if (cmd[0] == "PASS")
		//fonction pass
		*/
	}

}

/*--------------------------------------------------------*/

bool	Commands::passCmd(std::vector<std::string> line, int cl, Client *user, Server *server)
{
	std::string		msg;
	
	if (line[1].find("\n") > 0)
		line[1].erase(line[1].size() - 1);
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
		server->getFdUsersDc().push_back(cl);
		msg = "No try left: User is disconnected.\n";
		send(cl, msg.c_str(), msg.size(), 0);
		std::cout << "Client " << cl << " has been disconnected." << std::endl;
		server->clientDisconnected();
	}
	else
	{
		msg = "It left you " + std::to_string(3 - user->get_pass_try()) + " try.\n";
		send(cl, msg.c_str(), msg.size(), 0);
	}
	return (false);
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

void	Commands::privMsgCmd(std::string const &buf, int user_talk, Client *user, Server *server)
{
	std::string	msg;
	int			fd_received;
	
	msg = "Message of [ID: " + std::to_string(user_talk) + "] : " + buf;
	if (user->isConnected() == true)
	{
		std::cout << "je suis renteeee====" << std::endl;
		for(std::map<int, Client *>::iterator ite = server->_list_client.begin(); ite != server->_list_client.end(); ite++)
		{
			fd_received = ite->first;
			if (fd_received != user_talk && server->_list_client[fd_received]->passwordIsSet() == true)
			{
				if (send(fd_received, msg.c_str(), msg.length(), 0) < 0)
				{
					std::cout << "Send failed" << std::endl;
					server->getFdUsersDc().push_back(user_talk);
					std::cout << "Client " << user_talk << " has been disconnected." << std::endl;
				}
			}
		}
		if (user)
			server->_historic.push_back(msg);
	}
	else if(user->passwordIsSet() == false || user->userIsSet() == false)
	{
		msg = "You need to enter the password first then your user name.\n";
		send(user_talk, msg.c_str(), msg.size(), 0);
	}
}