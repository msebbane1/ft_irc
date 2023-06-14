/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:09:30 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/14 13:40:24 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Commands/Commands.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server;
class Client;
class Channel;

class Messages
{
	public :
	Messages();
	~Messages();
	void	welcome(Client *user, int fd);
	void	welcomeMsg(std::string user, std::string nick, int fd);
	void	errorMsg(std::string msg);
	void	displayMsgOnServer(std::string const &buf, int user_talk);

	//============ERR NICK=========///
	void	ERR_NOSUCHNICK(std::string nick, int fd);
	void	ERR_NONICKNAMEGIVEN(int fd);
	void	ERR_NICKNAMEINUSE(std::string nick, int fd);
	void	ERR_ERRONEUSNICKNAME(std::string nick, int fd);

	//============ERR PASS && USER=========///
	void	ERR_NEEDMOREPARAMS(int fd);
	void	ERR_ALREADYREGISTRED(int fd);
	void	ERR_PASSWDMISMATCH(int fd);

	//============ERR PRIVMSG=========///
	void	ERR_NORECIPIENT(int fd);
	void	ERR_NOTEXTTOSEND(int fd);

	void	ERR_UNKNOWNCOMMAND(std::string cmd, int fd);
	void	ERR_NOTREGISTERED(int fd);

	//============ERR CHANNELS=========///
	void	ERR_NOSUCHCHANNEL(std::string channel, int fd);
	void	ERR_UMODUUNKNOWNFLAG(std::string cmd, int fd); //un mode inconnue 
	void 	ERR_CHANOPRIVSNEEDED(std::string cmd, int fd);
	void	ERR_USERNOTINCHANNEL(std::string nick, std::string channel, int fd);
	void	ERR_CANNOTSENDTOCHAN(std::string target, int fd);

	//============REPLY=========///
	void	RPL_YOUREOPER(std::string nick, int fd);
	void	RPL_LEFTCHANNEL(std::string nick, std::string user, std::string arg, int fd);
	void 	RPL_KICK(std::string nick, std::string user, std::string channel, std::string kick, std::string reason, int fd);
	

	void	RPL_INVITING(std::string nick, std::string user, std::string invited, std::string channel, int fd);
	void	RPL_INVITE(std::string nick, std::string user, std::string invited, std::string channel, int fd);
	void	RPL_NOTOPIC(Channel *c, int fd, std::string nickname);
	void	RPL_TOPIC(Channel *c, int fd, std::string nickname);
	void	RPL_PRIVMSGCHAN(std::string nick, std::string channel, std::string msg, Channel *chan, int fd);
	void	RPL_PRIVMSG(std::string nick, std::string channel, std::string msg, int fd);
	void	RPL_QUIT(std::string nick, std::string user, std::string reason, int fd);

	//============ERR & RPL JOIN=========///
	void	RPL_NAMREPLY(Channel *c, int fd);
	void	RPL_ENDOFNAMES(Channel *c, int fd, std::string nickname);
	void	ERR_CANNOTJOIN(int fd, std::string chann, int err);

	//============ERR & RPL INVITE=========///
	void	ERR_NOTONCHANNEL(int fd, std::string chann);
	void	ERR_USERONCHANNEL(int fd, std::string nick, std::string chann);
};