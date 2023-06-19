/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asahonet <asahonet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:47:29 by asahonet          #+#    #+#             */
/*   Updated: 2023/06/19 12:59:27 by asahonet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Channel
{
	private:
		std::string					_name;
		Client*						_creator;
		std::string					_key;
		std::string					_topic;
		std::map<int, Client *>		_list_user_co;
		std::map<int, Client *>		_list_operators;
		std::vector<std::string>	_list_banned;
		std::string					_password;
		int							_size_max;
		bool						_i_only;
		std::vector<std::string>	_list_inv;
		
	public:
		Channel(std::string name, Client* c);
		Channel(std::string name,  Client* c, std::string key);
		virtual	~Channel();

		Channel*	operator=(Channel const *c);

		void	banUser(std::string username);
		void	displayUsers();
		void	displayOp();
		bool	userIsInChann(int user);
		void	sendMsg(int user_talk, std::string msg);
		bool	isOperator(int fd);
		int		nbUserInChan();
		bool	isBanned(std::string nickname);
		bool	isInv(std::string nickname);

		std::string					getName();
		void						setName(std::string name);

		std::map<int, Client*>		getListUserCo();
		void						addUser(Client *cl, int fd_cl);
		void						removeUser(std::string username);

		std::map<int, Client*>		getListOp();
		void						addOperator(Client *cl, int fd_cl);
		void						removeOperator(std::string username);

		std::string					getTopic();
		void						setTopic(std::string topic);
		bool						topicIsSet();

		std::string					getKey();
		void						setKey(std::string key);

		int							getSizeMax();
		void						setSizeMax(int size);

		void						setInviteOnly(bool set);

		std::vector<std::string>	getListInv();
		void						addListInv(std::string nickname);
		void						removeListInv(std::string nickname);

		Client*						getCreator();
};