#include "Client.hpp"

Client::Client(): _pass_try(0), _connected(false)
{
	//set_user(_user);
}

Client::~Client()
{
	delete this->user;
}

void		Client::setConnected(bool connected)
{
	this->_connected = connected;
}

void		Client::set_fd(int const &fd) 
{
	this->_fd = fd;
}
Client		*Client::get_user()
{
	return(this->user);
}
int const & Client::get_fd() const {
	return (this->_fd);
}
void		Client::set_user(Client *userr)
{
	this->user = userr;
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