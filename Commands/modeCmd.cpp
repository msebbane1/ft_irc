/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 11:11:37 by clecat            #+#    #+#             */
/*   Updated: 2023/06/22 11:35:05 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
//installer irssi sur ordi maison 
//irssi : /connect localhost 8080 irc
// vrai serveur : CONNECT irc.dal.net 6667

//change the channels mode : parametre attendu : /mode <channel> <+ | -> <mode> [parametres]
//												 cmd / name_chan / indicateur / option / optionnel(username for operator, etc)
//												  0		1			2			  3			4
// id a gerer on channel: i, t, k, o, l; + , - 
// on user: i,
// gerer les multiple mode: /mode #chan +tns(channel en mode +t, +n, +s), cas possible : -b+i
/*--------------------- MODE ----------------------*/
void	Commands::printListCmd()
{
	std::vector<std::string>::iterator	it = this->_line_cmd.begin();
	for(; it != this->_line_cmd.end(); it++)
	{
		std::cout << "[Server] Command sent from server << " << CYAN << *it << Color << std::endl;
	}
}

void	Commands::modeCmd(){

	printListCmd();
	if (this->_line_cmd.size() == 1)
	{
		this->_msg->ERR_NEEDMOREPARAMS(this->_fd_user);
		return;
	}
	if (this->_line_cmd.size() == 2)
		return;
	if (this->_s->clientExist(this->_line_cmd[1]))
		modeOnUser();
	else if (chanExist(this->_line_cmd[1]))
		modeOnChannel();
	else
	{
		this->_msg->ERR_NOSUCHNICK(this->_line_cmd[1], this->_fd_user);
		return ;
	}
}

char	Commands::findIndice()
{
	char	c = '\0';

	if(this->_line_cmd[2].size() == 1)
		return c;
	else
	{
		c = this->_line_cmd[2][0];
		if(isalpha(c) || isdigit(c))
			return '\0';
	}
	return c;
}

void		Commands::splitOption()
{
	for(int i = 0; i < (int)this->_line_cmd[2].size(); i++)
	{
		if((i == 0 && (this->_line_cmd[2][0] == '+' || this->_line_cmd[2][0] == '-')))
			continue;
		this->_optionList.push_back(this->_line_cmd[2][i]);
	}
}

int		Commands::ft_stoi( std::string & s )
{
	int i = 0;
	std::istringstream(s) >> i;
	return i;
}

//verifie les parametres
int		Commands::verifModeParam()
{
	if(!chanExist(this->_line_cmd[1])){
		this->_msg->ERR_NOSUCHCHANNEL(this->_line_cmd[1], this->_fd_user);
		return 1;
	}
	if(!this->_s->getChannel(this->_line_cmd[1])->userIsInChann(this->_fd_user)) //user not on channel 
	{
		this->_msg->ERR_NOTONCHANNEL(this->_user->getNickname(), this->_line_cmd[1], this->_fd_user);
		return 1;
	}
	if(!this->_s->getChannel(this->_line_cmd[1])->isOperator(this->_fd_user)) // verif if user isOperator in chan
	{
		this->_msg->ERR_CHANOPRIVSNEEDED(this->_line_cmd[0], this->_fd_user); // ERR_CHANOPRIVSNEEDED 482
		std::cout << " >> " << RED << this->_user->getNickname() << " not operator channel " << Color << std::endl;
		return 1;
	}
	char Indice = findIndice();
	if( Indice != '+' && Indice != '-'){
		setIndice('\0');
		return 1;
	}
	this->setIndice(Indice);
	splitOption();
	if(verifUser() == 1)
		return 1;
	return 0;
}

//message envoyé sur irssi quand le user ban est partie du channel et le essaie de le rejoindre : -!- Cannot join to channel #ko (You are banned)
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