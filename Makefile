#********************************************************************#

NAME 	= ircserv
GPP		= @c++
FLAGS	= -Wall -Werror -Wextra -std=c++98 -pedantic
MK		= @mkdir -p
RM 		= @rm -rf
FILE	= $(shell ls -lR | grep -F .cpp | wc -l)
CMP		= 1
CLEAR 	= @clear

SRC_DIR := src
SOURCES := main.cpp Channel.cpp Client.cpp Server.cpp commandParsing.cpp cmdNick.cpp cmdPass.cpp cmdQuit.cpp cmdUser.cpp cmdPrivmsg.cpp Authentification.cpp cmdJoin.cpp cmdCap.cpp cmdKick.cpp cmdInvite.cpp cmdTopic.cpp cmdMode.cpp cmdPart.cpp cmdPing.cpp cmdPong.cpp
SRC		:= $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJ_DIR := .objs
OBJ		:= $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))


#********************************************************************#

BOLD	= 	\e[1m
FAINT	=	\e[2m
ITA		=	\e[3m
BLINK	=	\e[5m
GREEN	=	\e[38;5;76m
RED		=	\e[38;5;196m
YELLOW	=	\e[38;5;227m
ORANGE	=	\e[38;5;208m
PURPLE	=	\e[38;5;201m
LBLUE	=	\e[38;5;45m
BLUE	=	\e[38;5;27m
INDI	=	\e[38;5;91m
SPINK	=	\e[38;5;225m
GREY	=	\e[38;5;254m
RESET	=	\e[00m

#********************************************************************#

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	$(MK) $(@D)
	@if [ "$(CMP)" -eq 1 ]; then \
		printf "\n"; \
	fi
	@printf "\r$(FAINT)$(SPINK)Compiling $(RESET)$(GREEN)$<$(BLUE) [$(SPINK)$(CMP)$(BLUE)/$(SPINK)$(FILE)$(BLUE)]$(RESET)                       \r"
	$(GPP) $(FLAGS) -c $< -o $@
	@$(eval CMP=$(shell echo $$(($(CMP)+1))))

$(NAME): $(OBJ)
	$(GPP) $(OBJ) $(FLAGS) -o $(NAME)
	@printf "                                                                    \r$(GREEN)compilationWasSuccessful$(RESET)\n"

clean:
	$(RM) $(NAME)
	@printf "\r\n\r$(GREEN)cleaned\n$(RESET)"

fclean:
	$(RM) $(OBJ_DIR) $(NAME)
	@printf "\r\n\r$(RED)fullyCleaned\n$(RESET)"

re: fclean $(NAME)

.PHONY: all clean fclean re
