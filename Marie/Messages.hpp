#pragma once

#include "Commands.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;



class Messages
{
	public :
	Messages();
	~Messages();
	void	welcome(Client *user, int fd);
	void	welcomeMsg(std::string user, std::string nick, int fd);
	void	errorSend(std::string num, std::string nick, std::string line, int fd) ;
	void	errorSendBuf(std::string num, std::string nick, std::string arg, std::string line, int fd) ;
	void	errorMsg(std::string msg);
	void	displayMsgOnServer(std::string const &buf, int user_talk);

	void	ERR_NEEDMOREPARAMS(int fd);

	//============ERR PRIVMSG=========///
	void	ERR_NOSUCHNICK(int fd);

};