#pragma once

///////INCLUDES//////
#include "Server.hpp"

///////DEFINES//////
#define VALID 1
#define INVALID 0

class Commands;

class Client
{
	private :
		int			_fd;
		int			_pass_try;
		bool		_password;
		std::string	_nickname;
		std::string	_user;
		//std::string	_mode;
		//std::string _real_name;

    public :
        Client();
        ~Client();

		int			get_pass_try();
		void		increment_pass_try();
		void		setNickname(std::string nickname);
		std::string getUser();
		std::string getNickname();
		void		setUser(std::string user);
		bool		isConnected();
		bool		nicknameIsSet();
		bool		userIsSet();
		bool		passwordIsSet();
		void		setPassword();
		int 		get_fd();
		void 		set_fd(int fd);
};
