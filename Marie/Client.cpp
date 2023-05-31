#include "Client.hpp"

Client::Client(): _pass_try(0), _password(false){}

Client::~Client(){}

int			Client::get_pass_try()
{
	return (this->_pass_try);
}

void		Client::increment_pass_try()
{
	this->_pass_try++;
}

bool		Client::passwordIsSet()
{
	return (this->_password);
}

void		Client::setPassword()
{
	if (this->_password == false)
		this->_password = true;
	else
		this->_password = false;
}


std::string Client::getNickname()
{
	return (this->_nickname);
}

void	Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

//======REALNAME=====///
std::string Client::getRealname()
{
	return (this->_real_name);
}

void	Client::setRealname(std::string realname)
{
	this->_real_name = realname;
}

bool	Client::realnameIsSet()
{
	if (this->_real_name.empty())
		return false;
	else
		return true;
}


std::string Client::getUser()
{
	return (this->_user);
}

void	Client::setUser(std::string realname)
{
	this->_user = realname;
}

bool	Client::userIsSet()
{
	if (this->_user.empty())
		return false;
	else
		return true;
}

int Client::get_fd()
{
	return (_fd);
}

void Client::set_fd(int fd) 
{
	_fd = fd;
}

bool	Client::nicknameIsSet()
{
	if (this->_nickname.empty())
		return false;
	else
		return true;
}

bool	Client::isConnected()
{
	if (nicknameIsSet() == true && passwordIsSet() == true && userIsSet())
		return true;
	else
		return false;
}