/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 11:11:37 by clecat            #+#    #+#             */
/*   Updated: 2023/06/06 13:39:50 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

/*--------------------- MODE ----------------------*/
//change the channels mode : parametre attendu : /mode <channel> <+ | -> <mode> [parametres]
//												 cmd / name_chan / indicateur / option / optionnel(username for operator, etc)
// id a gerer: i, t, k, o, l; + to do the action, - to undo
// gerer les multiple mode ? ex: /mode #chan +tns(channel en mode +t, +n, +s)
void	Commands::modeCmd(){

	std::vector<std::string>::iterator	it = this->_line_cmd.begin();
	for(int i = std::distance( this->_line_cmd.begin(), it ); it != this->_line_cmd.end(); ++it){
		std::string msg = this->_line_cmd[i];
		send(this->_s->getFdServer(), msg.c_str(), msg.size(), 0);
	}
	if( _line_cmd.size() < 4) //verifier le nombre de param (entre 4 et 5)
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
	if(!verifModeParam())
		return;
	//int		nb_opt = countOption();
	char	option = 'l';// split pour plusieurs options
	//faire une boucle sur nb_opt
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
	
		// default: // ? - voir doc
		// 	break;
	}
}

//verifie les parametres
int		Commands::verifModeParam(){

	if(!chanExist(this->_line_cmd[1])) // check if chan exist
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
	if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) //check if user is Operator on chann
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[1], this->_fd_user);
	//checker si indicateur present (3eme arg) // - ou +
	char Indice = findIndice();
	if( Indice != '+' && Indice != '-')
		std::cout << "ERR_WRONGARG" << std::endl;
	this->setIndice(Indice);
	//checker l'option(mode) (4eme arg) i, t, k, o, l; gerer les multipples modes ex: +itk
	//checker si le user existe(5eme arg)
	return 0;
}

//verifier si le User est OP pout chaque mode sauf o

// i : Set/remove Invite-only channel(no param needed)
void	Commands::setChanInviteOnlyMode(){
	if(this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // ERR_CHANOPRIVSNEEDED 482
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user);
	// indice : + ou - , verification faites en amont
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[2])->setInviteOnly(true);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[2])->setInviteOnly(false);
}

// t : Set/remove the restrictions of the TOPIC command to channel operators(topic protection, seuls les ChannelOperator peuvent changer le topic.)(no param needed)
void	Commands::setChanRestrictTopic(){
	// a gerer en fonction de la commande topic : donne ou enleve des restrictions
	//voir quels types de restrictions (ne pas voir le topic ou ne pas pouvoir le changer)
}

// k : Set/remove the channel key (password) (param needed) /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
void	Commands::setChanKey(){
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[2])->setKey(this->_line_cmd[4]);
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[2])->setKey("");
}

// o : Give/take channel operator privilege (param needed) /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
void	Commands::setChanOperator(){
	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[2])->addOperator(this->_user, this->_fd_user);// donner le Client et son fd;
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[2])->removeOperator(this->_line_cmd[4]);
}

// l : Set/remove the user limit to channel (param needed) /mode #cool +l 20 (limite le channel #cool à 20 utilisateurs)
void	Commands::setChanLimit(){

	if(this->getIndice() == '+')
		this->_s->getChannel(this->_line_cmd[3])->setSizeMax(20); // cast la taille en int a partir de _line_cmd
	else if (this->getIndice() == '-')
		this->_s->getChannel(this->_line_cmd[3])->setSizeMax(-1);
}

int		Commands::countOption(){
	int i = 0;
	return i;
}

char	Commands::findIndice(){

	char	c = '+';
	
	return c;
}

// Message d'erreur + code d'erreur
// ERR_NEEDMOREPARAMS 461 : quand il manque un paramètre à la commande.
// RPL_CHANNELMODEIS 324 : indique les modes possibles pour les chans.
// ERR_CHANOPRIVSNEEDED 482 : renvoyée quand vous tentez d'effectuer une opération d'administration sur un chan où vous n'êtes pas op
// ERR_NOSUCHNICK 401 : quand le nick n'a pas été trouvé sur le réseau.
// ERR_NOTONCHANNEL 442 : renvoyée quand vous n'êtes pas présent sur le chan.
// ERR_KEYSET 467 : renvoyée quand la clé du chan est déjà définie
// RPL_BANLIST 367 : indique un masque d'utilisateur qui est banni du chan.
// RPL_ENDOFBANLIST 368 : indique que tous les masques d'utilisateurs bannis du chan ont été indiqués.
// ERR_UNKNOWNMODE 472 : quand un code de mode de chan inconnu est utilisé.
// ERR_NOSUCHCHANNEL 403 : quand le chan n'a pas été trouvé sur le réseau

// ERR_USERSDONTMATCH 502 :
// RPL_UMODEIS 221 : renvoyée quand vous tentez d'effectuer une commande MODE sur un nick autre que le vôtre.
// 501 ERR_UMODUUNKNOWNFLAG : Cette erreur est renvoyée quand vous envoyez une commande MODE sur un utilisateur avec un mode inconnu.