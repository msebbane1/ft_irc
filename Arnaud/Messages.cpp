/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:09:13 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/02 17:11:18 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages(){}

Messages::~Messages(){}

//==============================================WELCOME MSG==================================================///

void	Messages::welcome(Client *user, int fd)
{
	std::string msg = ":localhost 001 " + user->getNickname() + "\r\n" 
	+ "\"Welcome to the Internet Relay Chat Network " + user->getNickname() + "!" + user->getUser()
	+ "@localhost" + "\"" + "\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//==============================================ERROR NICKNAME==================================================///

void	Messages::ERR_NOSUCHNICK(std::string nick, int fd) // 401
{
	std::string msg = ":irc.com 401 ERR_NEEDMOREPARAMS " + nick + " :No such nick/channel\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_NONICKNAMEGIVEN(int fd) // 431
{
	std::string msg = ":irc.com 431 ERR_NONICKNAMEGIVEN :No nickname given\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
	return ;
}

void	Messages::ERR_NICKNAMEINUSE(std::string nick, int fd) // 433
{
	std::string msg = ":irc.com 433 ERR_NICKNAMEINUSE " + nick + " :Nickname is already in use\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
	return ;
}

void	Messages::ERR_ERRONEUSNICKNAME(std::string nick, int fd) // 432
{
	std::string msg = ":irc.com 432 ERR_ERRONEUSNICKNAME " + nick + " :Erroneus nickname\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
	return ;
}

//==============================================ERROR PASSWORD && USER==================================================///

void	Messages::ERR_NEEDMOREPARAMS(int fd) // 461
{
	std::string msg = ":irc.com 461 ERR_NEEDMOREPARAMS :Not enough parameters\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_ALREADYREGISTRED(int fd) // 462
{
	std::string msg = ":irc.com 462 ERR_ALREADYREGISTRED :You may not reregister\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_PASSWDMISMATCH(int fd) // 464
{
	std::string	msg = ":irc.com 464 ERR_PASSWDMISMATCH :Password incorrect\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//==============================================ERROR PRIVMSG==================================================///


void Messages::ERR_NORECIPIENT(int fd) // 411
{
	std::string msg = ":irc.com 411 ERR_NORECIPIENT :No recipient given (<command>)\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void Messages::ERR_NOTEXTTOSEND(int fd) // 412
{
	std::string msg = ":irc.com 412 ERR_NOTEXTTOSEND :No text to send\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}



void Messages::ERR_UNKNOWNCOMMAND(std::string cmd, int fd) // 421
{
	std::string msg = ":irc.com 421 ERR_UNKNOWNCOMMAND " + cmd + " :Unknown command\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void Messages::ERR_NOTREGISTERED(int fd) // 451
{
	std::string msg = ":irc.com 451 ERR_NOTREGISTERED :You have not registered\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//====================CHANNELS===========//

void Messages::ERR_NOSUCHCHANNEL(std::string channel, int fd) // 403
{
	std::string msg = ":irc.com 403 ERR_NOSUCHCHANNEL " + channel + " :No such channel\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//====================REPLY================//

void Messages::RPL_YOUREOPER(std::string nick, int fd) // 403
{
	std::string msg = ":irc.com 381 RPL_YOUREOPER " + nick + " :You are now an IRC operator\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::welcomeMsg(std::string user, std::string nick, int fd)
{
	std::string msg = ":irc.com localhost 001 " + nick + "\r\n" 
	+ "\"Welcome to the Internet Relay Chat Network " + nick + "!" + user
	+ "@localhost" + "\"" + "\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void		Messages::errorMsg(std::string msg)
{
    std::cout << Red << msg << Color << std::endl;
	exit(EXIT_FAILURE);
}

void		Messages::displayMsgOnServer(std::string const &buf, int user_talk)
{
	if (buf == "\n")
		return;
    std::cout << "|User: "<< user_talk << "| Message send :" << buf ;
}

//==============================================ERROR & REPLY JOIN==================================================///

void	Messages::RPL_NOTOPIC(Channel *c) // 331
{
	std::string	msg = ":irc.com 331 RPL_NOTOPIC " + c->getName() + " :No topic is set";
	c->sendMsg(-1, msg);
}

void	Messages::RPL_TOPIC(Channel *c) // 332
{
	std::string	msg = ":irc.com 332 RPL_TOPIC " + c->getName() + " :" + c->getTopic();
	c->sendMsg(-1, msg);
}

void	Messages::RPL_NAMREPLY(Channel *c) // 353
{
	std::map<int, Client*>				map = c->getListUserCo();
	int									i = 0;
	
	std::string	msg = ":irc.com 353 RPL_NAMREPLY " + c->getName() + " :[USERS " + c->getName() + "]\n";
	for (std::map<int, Client*>::iterator it = map.begin(); it != map.end(); it++)
	{
		std::string mode;
		if (c->isOperator(it->first) == true)
			mode = "+";
		else
			mode = " ";
		msg += "[[" + mode + "]" + it->second->getNickname() + "] ";
		i++;
		if (i % 4 == 0)
			msg += "\n";
	}
	c->sendMsg(-1, msg);
}

void	Messages::RPL_ENDOFNAMES(Channel *c) // 366
{
	std::string	msg = ":irc.com 366 RPL_ENDOFNAMES " + c->getName() + " :End of /NAMES list";
	c->sendMsg(-1, msg);
}