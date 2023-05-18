/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:41 by msebbane          #+#    #+#             */
/*   Updated: 2023/05/18 15:29:03 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"

Commands::Commands(){}

Commands::~Commands(){}

void	Commands::exec_cmd(Server *server, std::string buf, Client *user, int user_talk)
{
	 std::vector<std::string>	cmd = splitCustom(buf, ' ');
	 std::string				msg;

	if(server->isCommandIrc(cmd[0]) == false)
		return ;
	if (cmd[0] == "PASS")
	{
		if (user->getConnected() == false && passCmd(cmd, user_talk, user, server) == false)
		{
			msg = "You entered a wrong password.\n";
			send(user_talk, msg.c_str(), msg.size(), 0);
		}
	}
	else if (cmd[0] == "NICK")
	{
		//verifier nickname valide
		user->setNickname(cmd[1]);
		msg = "======nickname is set=====\n";
		send(user_talk, msg.c_str(), msg.size(), 0);
		std::cout << "======nickname is set=====" << std::endl;
	}
	else if (cmd[0] == "USER")
	{
		user->setUser(cmd[1]);
		msg = "======USER is set=====\n";
		send(user_talk, msg.c_str(), msg.size(), 0);
		std::cout << "=====user is set====" << std::endl;
		return ;
	}
	/*
    else if (cmd[0] == "JOIN")
        //join_cmd(cmd, user, server);
	else if (cmd[0] == "PASS")
		//fonction pass
	*/

}

/*--------------------------------------------------------*/

bool	Commands::passCmd(std::vector<std::string> line, int cl, Client *user, Server *server)
{
	std::string		msg;
	
	if (line[1].find("\n") > 0)
		line[1].erase(line[1].size() - 1);
	if (line[1] == server->getPassword())
	{
		user->setConnected();
		msg = "You are connected!\n";
		send(cl, msg.c_str(), msg.size(), 0);
		//if (this->_historic.size() > 1)
			//sendHistoric(cl);
		return (true);
	}
	user->increment_pass_try();
	if (user->get_pass_try() == 3)
	{
		//this->_fd_users_dc.push_back(cl);
		msg = "No try left: User is disconnected.\n";
		send(cl, msg.c_str(), msg.size(), 0);
	}
	else
	{
		msg = "It left you " + std::to_string(3 - user->get_pass_try()) + " try.\n";
		send(cl, msg.c_str(), msg.size(), 0);
	}
	return (false);
}
/*--------------------------------------------------------*/
/*
bool	Commands::cmdForRegister(std::string buf, int cl, Server *server, std::string line, int user_talk)
{
	
	if (line == "PASS")
	{
		if (this->_list_client[cl]->getConnected() == false && passCmd(line, cl) == false)
		{
			msg = "You entered a wrong password.\n";
			send(cl, msg.c_str(), msg.size(), 0);
			return false;
		}
		return true;
	}
	else if (line == "NICK")
	{
		//verifier nickname valide
		_list_client[cl]->setNickname(line[1]);
		msg = "======nickname is set=====\n";
		send(cl, msg.c_str(), msg.size(), 0);
		std::cout << "======nickname is set=====" << std::endl;
		return true;
	}
	return false;
}
*/

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