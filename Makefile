NAME = ircserv
BONUS_NAME = ircserv_bonus
BONUS_BOT = ircserv_bonus_bot
SRC = SRCS/main.cpp SRCS/Argcontrols.cpp SRCS/IrcServer.cpp SRCS/CommandExecuter.cpp
BONUS_SRCS = SRCS_BONUS/main.cpp SRCS_BONUS/Argcontrols.cpp SRCS_BONUS/IrcServer.cpp SRCS_BONUS/CommandExecuter.cpp
BONUS_SRCS_BOT = SRCS_BONUS/Bot.cpp
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf
OBJ = $(SRC:.cpp=.o)
BONUS_OBJ = $(BONUS_SRCS:.cpp=.o)
BONUS_OBJ_BOT = $(BONUS_SRCS_BOT:.cpp=.o)

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
	@echo "IRC Compiled"
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

bonus : $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_SRCS) $(BONUS_OBJ) 
	$(CPP) $(CPPFLAGS) $(BONUS_SRCS) -o $(BONUS_NAME)
	@echo "$(LIGHT_GREEN)"
	@echo "IRC Bonus Compiled"
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

bonus_bot : $(BONUS_BOT)

$(BONUS_BOT): $(BONUS_SRCS_BOT) $(BONUS_OBJ_BOT)
	$(CPP) $(CPPFLAGS) $(BONUS_SRCS_BOT) -o $(BONUS_BOT)
	@echo "$(LIGHT_GREEN)"
	@echo "IRC Bonus Bot Compiled"
	@echo "$(RESET)"

clean:
	@clear
	@$(RM) $(OBJ)
	@echo "$(YELLOW)Object files removed$(RESET)"

clean_bonus:
	@clear
	@$(RM) $(BONUS_OBJ)
	@echo "$(YELLOW)Object files removed$(RESET)"

clean_bonus_bot:
	@clear
	@$(RM) $(BONUS_OBJ_BOT)
	@echo "$(YELLOW)Object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Executable removed$(RESET)"

fclean_bonus: clean_bonus
	@$(RM) $(BONUS_NAME)
	@echo "$(RED)Executable removed$(RESET)"

fclean_bonus_bot: clean_bonus_bot
	@$(RM) $(BONUS_BOT)
	@echo "$(RED)Executable removed$(RESET)"

re_bonus: fclean_bonus bonus

re_bonus_bot: fclean_bonus_bot bonus_bot

re: fclean all

.PHONY: all clean fclean re bonus clean_bonus fclean_bonus re_bonus bonus_bot clean_bonus_bot fclean_bonus_bot re_bonus_bot