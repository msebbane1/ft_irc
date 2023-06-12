/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeChanCmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 14:30:56 by clecat            #+#    #+#             */
/*   Updated: 2023/06/12 15:42:53 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//gestion Mode sur Channel
//change the channels mode : parametre attendu : /mode <channel> <+ | -> <mode> [parametres]
//												 cmd / name_chan / indicateur / option / optionnel(username for operator, etc)
//												  0		1			2			  3			4
// id a gerer: i, t, k, o, l; + to do the action, - to undo
// gerer les multiple mode: /mode #chan +tns(channel en mode +t, +n, +s), cas possible : -b+i

void	Commands::modeOnChannel(){

	verifModeParam();
	std::vector<char>::iterator	it = this->_optionList.begin();
	for(; it != this->_optionList.end(); ++it)
	{
		char option = *it;
		std::cout << "option: " << option << std::endl;
		std::map<int, Client*> UserCo = this->_s->getChannel(this->_line_cmd[1])->getListUserCo();
		std::cout << "erreur de malloc" << std::endl;
		if(this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user) == true)
		{
			for (std::map<int, Client*>::iterator it = UserCo.begin(); it != UserCo.end(); it++) 
			{
				std::string msg = ":" + this->_user->getNickname() + " " + this->_line_cmd[0] + " " + this->_line_cmd[1] + " " + this->_line_cmd[2] + " " + this->_line_cmd[3] + "\r\n"; // : utilisateurduchann + " COMMANDE " + ARG
				if(send(it->first, msg.c_str(), msg.length(), 0) < 0)
					this->_msg->errorMsg("failed send");
			}
		}
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
				setChanLimit(); //mode/#ko [+l 30] by clecat
				break;
			
			case 'b' :
				banUser();//ban user 
				break;
		
			default:
				this->_msg->ERR_UMODUUNKNOWNFLAG(this->_line_cmd[0], this->_fd_user); //checker l'option(mode) i, t, k, o, l; 
				break;
		}
	}
	return;
}

//verifie les parametres
int		Commands::verifModeParam(){

	if(!chanExist(this->_line_cmd[1])) // check if chan exist
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
	// if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) //check if user is Operator on chann
	// 	this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[1], this->_fd_user);
	char Indice = findIndice();
	if( Indice != '+' && Indice != '-')
		std::cout << "ERR_WRONGARG" << std::endl; // trouver l'erreur pour ce cas
	this->setIndice(Indice);
	splitOption(); //gerer les multipples modes ex: +itk
	// int usertofind; // faire fonction pour recuperer le fd du client donné
	// if(!this->_s->getChannel(this->_line_cmd[1])->userIsInChann(usertofind))
		
	return 0;
}

// i : Set/remove Invite-only channel(no param needed)
void	Commands::setChanInviteOnlyMode(){
	if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
	// indice : + ou - , verification faites en amont
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->setInviteOnly(true);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->setInviteOnly(false);
}

// t : Set/remove the restrictions of the TOPIC command to channel operators(topic protection, seuls les ChannelOperator peuvent changer le topic.)(no param needed)
void	Commands::setChanRestrictTopic(){
	if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
	// a gerer en fonction de la commande topic : donne ou enleve des restrictions
	//voir quels types de restrictions (ne pas voir le topic ou ne pas pouvoir le changer)
}

// k : Set/remove the channel key (password) (param needed) /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
void	Commands::setChanKey(){
	if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->setKey(this->_line_cmd[3]);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->setKey("");
}

// o : Give/take channel operator privilege (param needed) /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
void	Commands::setChanOperator(){
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->addOperator(this->_user, this->_fd_user);// donner le Client et son fd;
	else if (this->getIndice() == '-'){
		if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
			this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
		this->_s->getChannel(this->_line_cmd[1])->removeOperator(this->_line_cmd[3]);
	}
}

//segfault when connect with irssi on same computer with join channel 
// l : Set/remove the user limit to channel (param needed) /mode #cool +l 20 (limite le channel #cool à 20 utilisateurs)
void	Commands::setChanLimit(){

	if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user))// ERR_CHANOPRIVSNEEDED 482
	{
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
		return;
	}
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->setSizeMax(ft_stoi(this->_line_cmd[3])); // cast la taille en int a partir de _line_cmd
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->setSizeMax(-1);
}

// b : ban or deban a user on channel
void	Commands::banUser()
{
	if(this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[1])->banUser(this->_line_cmd[3]);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[1])->unbanUser(this->_line_cmd[3]);
}