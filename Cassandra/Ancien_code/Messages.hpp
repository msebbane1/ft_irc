/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:09:30 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/05 13:20:44 by asahonet         ###   ########.fr       */
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

	//============REPLY=========///
	void	RPL_YOUREOPER(std::string nick, int fd);

	//============ERR & RPL JOIN=========///
	void	RPL_NOTOPIC(Channel *c);
	void	RPL_TOPIC(Channel *c);
	void	RPL_NAMREPLY(Channel *c);
	void	RPL_ENDOFNAMES(Channel *c);
	void	ERR_BADCHANNELKEY(int fd, std::string chann);
	void	ERR_CHANNELISFULL(int d, std::string chann);
};