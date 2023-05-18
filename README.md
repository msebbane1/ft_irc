# ft_IRC : Serveur IRC en C++98

# Lien Utils
# Details du sujet
# Infos


# 🔗: Lien Utils

	*Def IRCop
#	https://fr.wikipedia.org/wiki/IrcOp
	*IRCop
#	https://www.unrealircd.org/docs/IRCOp_guide/fr#Avant-propos
	*Doc command IRC
#	https://irc.dalexhd.dev/
	*Derniere update doc IRC
#	https://www.rfc-editor.org/rfc/rfc2812
	*Meilleur doc pour IRC
#	https://modern.ircdocs.horse/
	* List command possible
#	https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands
	*Tuto IRC(serveur, etc)
#	https://ircgod.com/
	*Man Socket
#	http://manpagesfr.free.fr/man/man2/socket.2.html
	*Man Socket2
#	https://www.geeksforgeeks.org/socket-programming-cc/
	*Lien qui va nous servir (juste de rien enfaite)
#	https://emmanuel-delahaye.developpez.com/tutoriels/c/notes-reseaux-c/#LII-B-1

# Details du Sujet :

	*Creer un seveur IRC pouvant gerer plusieurs clients simultanément;
	*Ne pas gerer la communication de serveur a serveur
	*Le forking est interdit;
	*Choisir un client IRC comme reference;
	*Fonction autorisée :
		Tout ce qui respecte la norme C++ 98.
		socket, close, setsockopt, getsockname,
		getprotobyname, gethostbyname, getaddrinfo,
		freeaddrinfo, bind, connect, listen, accept,
		htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa,
		send, recv, signal, lseek, fstat, fcntl, poll (ou
		équivalent)
