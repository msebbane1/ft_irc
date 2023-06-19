/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeChanCmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 14:30:56 by clecat            #+#    #+#             */
/*   Updated: 2023/06/19 14:09:25 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//gestion Mode sur Channel
//change the channels mode : parametre attendu : /mode <channel> <+ | -> <mode> [parametres]
//												 cmd / name_chan / indicateur / option / optionnel(username for operator, etc)
//												  0		1			2			  3			4
// id a gerer: i, t, k, o, l; + to do the action, - to undo
// gerer les multiple mode: /mode #chan +tns(channel en mode +t, +n, +s), cas possible : -b+i
//
void	Commands::modeOnChannel(){

	if(verifModeParam() == 1)
		return;
	if(getIndice() == '\0')
		return;
	std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo(); //mettre ailleur sd car affiche message 2 fois
	if(this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true)
	{
		for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
		{
			std::string msg = ":" + this->_user->getNickname() + " " + this->_line_cmd[0] + " " + this->_line_cmd[1] + " " + this->_line_cmd[2] + " " + this->_line_cmd[3] + "\r\n"; // : utilisateurduchann + " COMMANDE " + ARG
			if(send(it->first, msg.c_str(), msg.length(), 0) < 0)
				this->_msg->errorMsg("failed send");
		}
	}
	std::vector<char>::iterator	it = this->_optionList.begin();
	for(; it != this->_optionList.end(); ++it)
	{
		char option = *it;
		switch (option)
		{
			case 'i':
				setChanInviteOnlyMode();
				break;

			case 't' :
				setChanRestrictTopic();
				break;

			case 'k' :
				setChanKey();
				break;

			case 'o' :
				setChanOperator();
				break;
			
			case 'l' :
				setChanLimit();
				break;
			
			case 'b' :
				banUser();
				break;
		
			default:
				this->_msg->ERR_UMODUUNKNOWNFLAG(this->_line_cmd[0], this->_fd_user); //checker l'option(mode) i, t, k, o, l; 
				break;
		}
	}
	return;
}

int		Commands::verifUser()
{
	std::vector<char>::iterator	it = this->_optionList.begin();
	for(; it != this->_optionList.end(); it++)
	{
		char	option = *it;
		if(option == 'o')
		{
			if(this->_line_cmd.size() == 3) //si pas de user donner ignorer
				return 1;
			if(getIndice() == '+' && this->_line_cmd.size() != 3)
			{
				if(!this->_s->getChannel(this->_line_cmd[1])->userIsInChan(this->_line_cmd[3]))//verif if user in chan for mode b/o
				{
					this->_msg->ERR_NOSUCHNICK(this->_line_cmd[3], this->_fd_user);
					this->_msg->ERR_USERNOTINCHANNEL(this->_line_cmd[3], this->_line_cmd[1], this->_fd_user);
					return 1;
				}
				else if(this->_s->getChannel(this->_line_cmd[1])->userIsInChan(this->_line_cmd[3]) && this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_line_cmd[3]) == true)
					return 1;
			}
		}
		else if(option == 'b')
		{
			if(this->_line_cmd.size() == 3)
			{
				if(this->_s->getChannel(this->_line_cmd[1])->getListUserBanned().size() == 0){
					this->_msg->RPL_ENDOFBANLIST(this->_line_cmd[1], this->_fd_user);
					return 1;
				}
				else
				{
				// 	// int size = this->_s->getChannel(this->_line_cmd[1])->getListUserBanned().size();
				// 	// std::vector<std::string>::iterator	ite = this->_s->getChannel(this->_line_cmd[1])->getListUserBanned().end() - 1;
				// 	// for(; ite >= this->_s->getChannel(this->_line_cmd[1])->getListUserBanned().begin() && size > 0; ite--)
				// 	// {
				// 	// 	std::cout << "dans la boucle + ite: " << *ite << " + size: " << size << std::endl;
				// 	// 	int num = --size;
				// 	// 	this->_msg->RPL_BANLIST(this->_fd_user, this->_line_cmd[1], *ite, num);
				// 	// }
				// 	// this->_msg->RPL_ENDOFBANLIST(this->_line_cmd[1], this->_fd_user);
					return 1;
				}
			}
			else if(this->_s->getChannel(this->_line_cmd[1])->isBanned(this->_line_cmd[3]) && getIndice() == '+')
				return 1;
			else if(!this->_s->getChannel(this->_line_cmd[1])->userIsInChan(this->_line_cmd[3]))//verif if user in chan for mode b/o
			{
				this->_msg->ERR_NOSUCHNICK(this->_line_cmd[3], this->_fd_user);
				this->_msg->ERR_USERNOTINCHANNEL(this->_line_cmd[3], this->_line_cmd[1], this->_fd_user);
				return 1;
			}
		}
	}
	return 0;
}

// i : Set/remove Invite-only channel(no param needed)
void	Commands::setChanInviteOnlyMode(){
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->setInviteOnly(true);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->setInviteOnly(false);
}

// topic protection, seuls les ChannelOperator peuvent changer le topic.
// t : Set/remove the restrictions of the TOPIC command to channel operators(topic protection, seuls les ChannelOperator peuvent changer le topic.)(no param needed)
void	Commands::setChanRestrictTopic(){

	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->setTopicProtected(true);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->setTopicProtected(false);
}

// k : Set/remove the channel key (password) (param needed) /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
void	Commands::setChanKey(){
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->setKey(this->_line_cmd[3]);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->setKey("");
}

// o : Give/take channel operator privilege (param needed) /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
void	Commands::setChanOperator(){
	// faire fonction pour verifier si le userr is in chan
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->addOperator(this->_s->getClient(this->_line_cmd[3]), this->_s->getClient(this->_line_cmd[3])->get_fd());// donner le Client et son fd;
	else if (this->getIndice() == '-'){
		if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
			this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
		this->_s->getChannel(this->_line_cmd[1])->removeOperator(this->_line_cmd[3]);
	}
}

// l : Set/remove the user limit to channel (param needed) /mode #cool +l 20 (limite le channel #cool à 20 utilisateurs)
void	Commands::setChanLimit(){

	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->setSizeMax(ft_stoi(this->_line_cmd[3])); // cast la taille en int a partir de _line_cmd
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->setSizeMax(-1);
}

// b : ban or deban a user on channel
void	Commands::banUser()
{
	// faire fonction pour verifier si le user is in chan
	if(this->getIndice() == '+'){
		this->_s->getChannel(this->_line_cmd[1])->banUser(this->_line_cmd[3]);
	}
	else if (this->getIndice() == '-'){
		this->_s->getChannel(this->_line_cmd[1])->unbanUser(this->_line_cmd[3]);
	}
}
//comportement a changer pour ban : le user ban doit recevoir les message mais ne peux plus les envoyer tant qu'il ne quitte pas le channel
//faire une verif pour part pour le remove de la list + user not part recois le message des autres condition privmsg