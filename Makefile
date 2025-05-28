# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/14 11:56:46 by olardeux          #+#    #+#              #
#    Updated: 2025/05/14 12:27:17 by olardeux         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = IRCServer
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f
SRCS = main.cpp Server.cpp
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# Colors
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)Compilation complete!$(RESET)"
	@echo "$(CYAN)Executable: $(NAME)$(RESET)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@echo -n "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "\rSuccessfully compiled $<$(RESET)"

clean:
	@echo "$(RED)Cleaning up...$(RESET)"
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@echo "$(GREEN)Cleaned up!$(RESET)"

fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)Executable removed!$(RESET)"
	
re: fclean all
	@echo "$(GREEN)Rebuilding...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)Rebuild complete!$(RESET)"

.PHONY: all clean fclean re