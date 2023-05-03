# ft_IRC : Serveur IRC en C++98

# Lien Utils
# Details du sujet
# Infos


# 🔗: Lien Utils

#		https://fr.wikipedia.org/wiki/IrcOp
#		https://www.unrealircd.org/docs/IRCOp_guide/fr#Avant-propos
#		https://irc.dalexhd.dev/
#		https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands

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
