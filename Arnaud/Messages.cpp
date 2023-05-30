#include "Messages.hpp"

Messages::Messages(){}

Messages::~Messages(){}


void	Messages::welcome(Client *user, int fd)
{
	std::string msg = ":localhost 001 " + user->getNickname() + "\r\n" 
	+ "\"Welcome to the Internet Relay Chat Network " + user->getNickname() + "!" + user->getUser()
	+ "@localhost" + "\"" + "\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_NEEDMOREPARAMS(int fd) // 461
{
	std::string msg = "461 ERR_NEEDMOREPARAMS :Not enough parameters\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::ERR_NOSUCHNICK(int fd) // 401
{
	std::string msg = ":irc.com 401 ERR_NOSUCHNICK : No such nick/channel\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}


void	Messages::welcomeMsg(std::string user, std::string nick, int fd)
{
	std::string msg = ":localhost 001 " + nick + "\r\n" 
	+ "\"Welcome to the Internet Relay Chat Network " + nick + "!" + user
	+ "@localhost" + "\"" + "\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::errorSend(std::string num, std::string nick, std::string line, int fd) 
{
	std::string msg = ":localhost " + num + " " + nick + " :" + line + "\r\n";
	if(send(fd, msg.c_str(), msg.length(), 0) < 0)
		errorMsg("failed send");
}

void	Messages::errorSendBuf(std::string num, std::string nick, std::string arg, std::string line, int fd) 
{
	std::string msg = ":localhost " + num + " " + nick + " " + line + " :" + arg + "\r\n";
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
    std::cout << "| USER : client " << user_talk << " |" << std::endl;
    std::cout << "Message send :" << buf ;
}