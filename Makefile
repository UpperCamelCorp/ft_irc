# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/14 11:56:46 by olardeux          #+#    #+#              #
#    Updated: 2025/06/04 12:36:18 by olardeux         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project settings
NAME        = IRCServer
CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98
RM          = rm -f
SRCS_FILES  = main.cpp\
			Server.cpp\
			Client.cpp
SRCS_DIR  	= srcs
SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))
INC         = -I srcs/inc
OBJ_DIR     = obj
OBJS        = $(addprefix $(OBJ_DIR)/, $(SRCS_FILES:.cpp=.o))

# Colors for output
RED         = \033[31m
GREEN       = \033[32m
YELLOW      = \033[33m
BLUE        = \033[34m
MAGENTA     = \033[35m
CYAN        = \033[36m
RESET       = \033[0m

# Stylish header
HEADER = $(CYAN)========================================$(RESET)
FOOTER = $(CYAN)========================================$(RESET)

# Targets
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(HEADER)"
	@echo "$(GREEN)Compiling project...$(RESET)"
	@$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJS)
	@echo "$(GREEN)Compilation complete!$(RESET)"
	@echo "$(CYAN)Executable: $(NAME)$(RESET)"
	@echo "$(FOOTER)"

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo -n "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@echo -e "\r$(GREEN)Successfully compiled $<$(RESET)"

clean:
	@echo "$(HEADER)"
	@echo "$(RED)Cleaning up object files...$(RESET)"
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@echo "$(GREEN)Object files cleaned!$(RESET)"
	@echo "$(FOOTER)"

fclean: clean
	@echo "$(HEADER)"
	@echo "$(RED)Removing executable...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)Executable removed!$(RESET)"
	@echo "$(FOOTER)"

re: fclean all
	@echo "$(HEADER)"
	@echo "$(MAGENTA)Rebuilding project...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)Rebuild complete!$(RESET)"
	@echo "$(FOOTER)"

.PHONY: all clean fclean re