/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:09:13 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/13 13:32:43 by clecat           ###   ########.fr       */
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


void	Messages::ERR_NORECIPIENT(int fd) // 411
{
	std::string msg = ":irc.com 411 ERR_NORECIPIENT :No recipient given (<command>)\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_NOTEXTTOSEND(int fd) // 412
{
	std::string msg = ":irc.com 412 ERR_NOTEXTTOSEND :No text to send\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}



void	Messages::ERR_UNKNOWNCOMMAND(std::string cmd, int fd) // 421
{
	std::string msg = ":irc.com 421 ERR_UNKNOWNCOMMAND " + cmd + " :Unknown command\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_NOTREGISTERED(int fd) // 451
{
	std::string msg = ":irc.com 451 ERR_NOTREGISTERED :You have not registered\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//====================CHANNELS===========//

void	Messages::ERR_NOSUCHCHANNEL(std::string channel, int fd) // 403
{
	std::string msg = ":irc.com 403 ERR_NOSUCHCHANNEL " + channel + " :No such channel\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//erreur pour MODE (si le client n'est pas OP sur le chan)
void	Messages::ERR_CHANOPRIVSNEEDED(std::string cmd, int fd) // 482
{
	std::string msg = ":irc.com 482 ERR_CHANOPRIVSNEEDED " + cmd + " :Not operator\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//unknown mode
void	Messages::ERR_UMODUUNKNOWNFLAG(std::string cmd, int fd) // 501
{
	std::string msg = ":irc.com 501 ERR_UMODUUNKNOWNFLAG " + cmd + " :Unknown mode\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//404 ERR_CANNOTSENDTOCHAN
void	Messages::ERR_CANNOTSENDTOCHAN(std::string target, int fd) //404
{
	std::string msg = ":irc.com 404 ERR_CANNOTSENDTOCHAN " + target + " :Cannot send to channel\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//====================REPLY================//

void Messages::RPL_YOUREOPER(std::string nick, int fd) // 381
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
	std::string	msg = ":irc.com 331 RPL_NOTOPIC " + c->getName() + " :No topic is set\r\n";
	c->sendMsg(-1, msg);
}

void	Messages::RPL_TOPIC(Channel *c) // 332
{
	std::string	msg = ":irc.com 332 RPL_TOPIC " + c->getName() + " :" + c->getTopic() + "\r\n";
	c->sendMsg(-1, msg);
}

void	Messages::RPL_NAMREPLY(Channel *c) // 353
{
	std::map<int, Client*>				map = c->getListUserCo();
	int									i = 0;
	
	std::string	msg = ":irc.com 353 RPL_NAMREPLY " + c->getName() + " :[USERS " + c->getName() + "]\r\n";
	for (std::map<int, Client*>::iterator it = map.begin(); it != map.end(); it++)
	{
		std::string mode;
		if (c->isOperator(it->first) == true)
			mode = "+";
		else
			mode = " ";
		msg += "[" + mode + it->second->getNickname() + "] ";
		i++;
		if (i % 4 == 0)
			msg += "\r\n";
	}
	msg += "\r\n";
	c->sendMsg(-1, msg);
}

void	Messages::RPL_ENDOFNAMES(Channel *c) // 366
{
	std::string	msg = ":irc.com 366 RPL_ENDOFNAMES " + c->getName() + " :End of /NAMES list\r\n";
	c->sendMsg(-1, msg);
}

void	Messages::ERR_CANNOTJOIN(int fd, std::string chann, int err) // 471, 473, 474, 475
{
	std::string	mode;
	std::string	type;
	
	if (err == 471)
	{
		type = " ERR_CHANNELISFULL ";
		mode = "(+l)\r\n";
	}
	else if (err == 473)
	{
		type = " ERR_INVITEONLYCHAN ";
		mode = "(+i)\r\n";
	}
	else if (err == 474)
	{
		type = " ERR_BANNEDFROMCHAN ";
		mode = "(+b)\r\n";
	}
	else if (err == 475)
	{
		type = " ERR_BADCHANNELKEY ";
		mode = "(+k)\r\n";
	}
	
	std::string	msg = ":irc.com " + /*err +*/ type + chann + " :Cannot join channel " + mode;
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

//========== ERROR ============//

void	Messages::ERR_NOTONCHANNEL(std::string nick, std::string channel, int fd) //442
{
	std::string msg = ":localhost 442 ERR_NOTONCHANNEL " + nick + " " + channel + " :You're not on that channel\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_USERONCHANNEL(int fd, std::string nick, std::string chann) // 443
{
	std::string	msg = ":irc.com 443 ERR_USERONCHANNEL " + chann + nick + " :is already on channel\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::RPL_LEFTCHANNEL(std::string nick, std::string user, std::string arg, int fd)
{
	std::string	msg = ":" +  nick + "!" + user + "@localhost " + "PART" + " :" + arg + "\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}