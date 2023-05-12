#include "Client.hpp"

Client::Client()
{
	//set_user(_user);
	_pass_try = 0;
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
int	Client::get_pass_try()
{
	return(_pass_try);
}
void Client::increment_pass_try()
{
	_pass_try++;
}
void	Client::set_user(Client *userr)
{
	user = userr;
}
Client::~Client()
{
	delete user;
}