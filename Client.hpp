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
		std::string _real_name;
		bool		_isInvisible;
		bool		_isIRCOperator;
		bool		_isBot;
		//std::string	_mode;

    public :
        Client();
        ~Client();

		//===========UTILS==========//
		bool		isConnected();
		int			get_pass_try();
		void		increment_pass_try();

		//===========GETTERS && SETTERS==========//
		std::string getRealname();
		void		setRealname(std::string user);
		bool		realnameIsSet();

		void		setNickname(std::string nickname);
		std::string getNickname();
		bool		nicknameIsSet();

		std::string getUser();
		void		setUser(std::string user);
		bool		userIsSet();

		bool		passwordIsSet();
		void		setPassword();

		int 		get_fd();
		void 		set_fd(int fd);

		bool		getInvisible() const;
		void		setInvisible(bool mode);

		bool		getIRCOperator() const;
		void		setIRCOperator(bool ircop);

		//===========BOT==========//
		bool	containBanWord(std::string str);
};
