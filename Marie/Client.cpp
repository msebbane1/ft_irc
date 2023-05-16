#include "Client.hpp"

Client::Client(): _pass_try(0), _connected(false){}

Client::~Client(){}

int			Client::get_pass_try()
{
	return(this->_pass_try);
}

void		Client::increment_pass_try()
{
	this->_pass_try++;
}



bool		Client::getConnected()
{
	return (this->_connected);
}

void		Client::setConnected()
{
	if (this->_connected == false)
		this->_connected = true;
	else
		this->_connected = false;
}

void	Client::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void Client::setUser(std::string user)
{
	_user = user;
}

bool Client::userIsSet()
{
	if(_user.empty())
		return false;
	else
		return true;
}

bool Client::nicknameIsSet()
{
	if(_nickname.empty())
		return false;
	else
		return true;
}

bool	Client::isConnected()
{
	if(nicknameIsSet() == true && userIsSet() == true)
	{
		return(this->_connected);
	}
	else
		return(false);
}
