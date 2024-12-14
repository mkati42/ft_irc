NAME = ircserv
SRC = SRCS/main.cpp SRCS/Argcontrols.cpp SRCS/IrcServer.cpp SRCS/CommandExecuter.cpp
CPP = g++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf
OBJ = $(SRC:.cpp=.o)

RESET=\033[0m
RED=\033[31m
LIGHT_RED=\033[91m
GREEN=\033[32m
LIGHT_GREEN=\033[92m
YELLOW=\033[33m
LIGHT_YELLOW=\033[93m
BLUE=\033[34m
LIGHT_BLUE=\033[94m
MAGENTA=\033[35m
LIGHT_MAGENTA=\033[95m
CYAN=\033[36m
LIGHT_CYAN=\033[96m
WHITE=\033[37m
GREY=\033[90m
LIGHT_GREY=\033[37m

all :$(NAME)

$(NAME): $(SRC) $(OBJ)

	$(CPP) $(CPPFLAGS) $(SRC) -o $(NAME)
	@echo "$(LIGHT_GREEN)"
	@echo "IRC Compailed"
	@echo "$(RESET)"
	@clear
	@echo "$(RED)"
	@echo " ██▓    ██▀███      ▄████▄  "
	@echo "▓██▒   ▓██ ▒ ██▒   ▒██▀ ▀█  "
	@echo "▒██▒   ▓██ ░▄█ ▒   ▒▓█    ▄ "
	@echo "░██░   ▒██▀▀█▄     ▒▓▓▄ ▄██▒"
	@echo "░██░   ░██▓ ▒██▒   ▒ ▓███▀ ░"
	@echo "░▓     ░ ▒▓ ░▒▓░   ░ ░▒ ▒  ░"
	@echo " ▒ ░     ░▒ ░ ▒░     ░  ▒   "
	@echo " ▒ ░     ░░   ░    ░        "
	@echo " ░        ░        ░ ░      "
	@echo "                   ░        ";
	@echo "$(RESET)"
	@echo "$(BLUE)				by uarslan & acan & mkati$(RESET)"

clean:
	@clear
	@$(RM) $(OBJ)
	@echo "$(YELLOW)Object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Executable removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re