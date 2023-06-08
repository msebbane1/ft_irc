/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeChanCmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 14:30:56 by clecat            #+#    #+#             */
/*   Updated: 2023/06/08 15:31:47 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

//gestion Mode sur Channel

void	Commands::modeOnChannel(){

	std::vector<char>::iterator	it = this->_optionList.begin();
	for(; it != this->_optionList.end(); it++)
	{
		char option = this->_optionList[*it];
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
				banUser();//ban user 
				break;
		
			default:
				this->_msg->ERR_UMODUUNKNOWNFLAG(this->_line_cmd[1], this->_fd_user); //checker l'option(mode) i, t, k, o, l; 
				break;
		}
	}
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
		this->_s->getChannel(this->_line_cmd[2])->setInviteOnly(true);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[2])->setInviteOnly(false);
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
		this->_s->getChannel(this->_line_cmd[2])->setKey(this->_line_cmd[4]);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[2])->setKey("");
}

// o : Give/take channel operator privilege (param needed) /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
void	Commands::setChanOperator(){
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[2])->addOperator(this->_user, this->_fd_user);// donner le Client et son fd;
	else if (this->getIndice() == '-'){
		if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
			this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
		this->_s->getChannel(this->_line_cmd[2])->removeOperator(this->_line_cmd[4]);
	}
}

// l : Set/remove the user limit to channel (param needed) /mode #cool +l 20 (limite le channel #cool à 20 utilisateurs)
void	Commands::setChanLimit(){

	if(this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[3])->setSizeMax(ft_stoi(this->_line_cmd[4])); // cast la taille en int a partir de _line_cmd
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[3])->setSizeMax(-1);
}

// b : ban or deban a user on channel
void	Commands::banUser()
{
	if(this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
	if(this->getIndice() == '+')
		std::cout << "User is Ban" << std::endl;
	else if (this->getIndice() == '-')
		std::cout << "User is UnBan" << std::endl;
}