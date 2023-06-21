CC = c++

RM = rm -f

CFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv

SRC = Client.cpp Server.cpp main.cpp Channel.cpp Messages.cpp Bot.cpp \
		Commands/Commands.cpp Commands/joinCmd.cpp Commands/nickCmd.cpp \
		Commands/passCmd.cpp Commands/privmsgCmd.cpp Commands/quitCmd.cpp \
		Commands/userCmd.cpp Commands/killCmd.cpp Commands/operCmd.cpp \
		Commands/inviteCmd.cpp Commands/partCmd.cpp Commands/topicCmd.cpp \
		Commands/kickCmd.cpp Commands/modeCmd.cpp Commands/modeUserCmd.cpp \
		Commands/modeChanCmd.cpp

OBJ = $(SRC:.cpp=.o)

all:
	@$(MAKE) -j $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

%.o : %.cpp
	$(CC) -c $(CFLAGS) $(SRC) $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re