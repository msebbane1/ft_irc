#include "Client.hpp"

Client::Client()
{
	//set_user(_user);
}

Client *Client::get_user()
{
	return(user);
}

void	Client::set_user(Client *userr)
{
	user = userr;
}
Client::~Client()
{
	delete user;
}