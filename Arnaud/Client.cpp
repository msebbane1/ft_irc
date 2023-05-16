#include "Client.hpp"

Client::Client(): _pass_try(0), _connected(false)
{
}

Client::~Client()
{
	delete this->user;
}

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