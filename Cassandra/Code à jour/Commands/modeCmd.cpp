/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clecat <clecat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 11:11:37 by clecat            #+#    #+#             */
/*   Updated: 2023/06/08 15:29:35 by clecat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
//installer irssi sur ordi maison 
//irssi : /connect localhost port mdp

//change the channels mode : parametre attendu : /mode <channel> <+ | -> <mode> [parametres]
//												 cmd / name_chan / indicateur / option / optionnel(username for operator, etc)
//												  0		1			2			  3			4
// id a gerer: i, t, k, o, l; + to do the action, - to undo
// gerer les multiple mode: /mode #chan +tns(channel en mode +t, +n, +s), cas possible : -b+i
/*--------------------- MODE ----------------------*/
void	Commands::modeCmd(){

	if( _line_cmd.size() < 2) //verifier le nombre de param (entre 3 et 4)
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user); //cette erreur fonctionne
	//faire un dispatch en foction d'un user ou d'un channel
	//if() //si _line_cmd[1] est un user
	//	modeOnUser();
	//else() //si un channel
	//	modeOnChannel();
	verifModeParam();
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
		
			default:
				this->_msg->ERR_UMODUUNKNOWNFLAG(this->_line_cmd[1], this->_fd_user); //checker l'option(mode) i, t, k, o, l; 
				break;
		}
	}
}



char	Commands::findIndice(){ // a faire plus propre

	char	c = '\0';

	if(this->_line_cmd[2].size() == 1)
		std::cerr << "Missing Option or Indice" << std::endl;
	else{
		c = this->_line_cmd[2][0];
		if(isalpha(c) || isdigit(c))
			std::cerr << "Missing Indice" << std::endl; // voir l'erreur adéquat
	}
	return c;
}

void		Commands::splitOption(){
	
	//remplir optionList par toutes les options: gestion option multiple
	for(int i = 0; i < (int)this->_line_cmd[2].size(); i++){
		
		if((i == 0 && (this->_line_cmd[2][0] == '+' || this->_line_cmd[2][0] == '-')))
			continue;
		this->_optionList.push_back(this->_line_cmd[2][i]);
	}
}

int		Commands::ft_stoi( std::string & s ){
	int i = 0;
	std::istringstream(s) >> i;
	return i;
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

// ERR_USERSDONTMATCH 502 : Cette erreur est renvoyée quand vous tentez d'effectuer une commande MODE sur un nick autre que le vôtre.
// RPL_UMODEIS 221 : renvoyée quand vous tentez d'effectuer une commande MODE sur un nick autre que le vôtre.
// 501 ERR_UMODUUNKNOWNFLAG : Cette erreur est renvoyée quand vous envoyez une commande MODE sur un utilisateur avec un mode inconnu.