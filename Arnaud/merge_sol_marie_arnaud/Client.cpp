#include "Client.hpp"

Client::Client()
{
	//set_user(_user);
}

void Client::set_fd(int const &fd) 
{
	_fd = fd;
}
Client *Client::get_user()
{
	return(user);
}
int const & Client::get_fd() const {
	return (_fd);
}
void	Client::set_user(Client *userr)
{
	user = userr;
}
Client::~Client()
{
	delete user;
}