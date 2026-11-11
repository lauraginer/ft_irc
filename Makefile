# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/02 20:36:28 by lauragm           #+#    #+#              #
#    Updated: 2026/11/11 19:31:17 by lauragm          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     := ircserv
CPP      := c++
CPPFLAGS := -Wall -Wextra -Werror -std=c++98 -g
# Para depurar a mano, descomenta:
# CPPFLAGS += -fsanitize=address,undefined

INCLUDES := -Iinclude
OBJS_DIR := build

SRCS := $(wildcard src/*.cpp src/commands/*.cpp) #temporal
OBJS := $(SRCS:%.cpp=$(OBJS_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

BLUE   = \033[0;34m
GREEN  = \033[0;32m
YELLOW = \033[0;33m
RED    = \033[0;31m
RESET  = \033[0m

.PHONY: all clean fclean re debug header

all: header $(NAME)
.PHONY: all clean fclean re debug

all: header $(NAME)

header:
	@echo "                                               "
	@echo "$(YELLOW)$(BOLD)"
	@echo "\t ███████╗████████╗     ██╗██████╗  ██████╗  "
	@echo "\t ██╔════╝╚══██╔══╝     ██║██╔══██╗██╔════╝  "
	@echo "\t █████╗     ██║        ██║██████╔╝██║       "
	@echo "\t ██╔══╝     ██║        ██║██╔══██╗██║       "
	@echo "\t ██║        ██║███████╗██║██║  ██║╚██████╗  "
	@echo "\t ╚═╝        ╚═╝╚══════╝╚═╝╚═╝  ╚═╝ ╚═════╝  "
	@echo "                                               "
	@echo "$(BLUE)                 🚀 42 School Project 🚀 "
	@echo "$(RED)                   juan-jof & lginer-m     "
	@echo "$(RESET)"
	
$(NAME): $(OBJS)
	@echo "$(GREEN)🔗 Linking $(NAME)...$(RESET)"
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) created!$(RESET)"
	
$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(BLUE)🧹 Object files cleaned!$(RESET)"
	
fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)🔥 $(NAME) has been deleted!$(RESET)"

re: fclean all

debug: CPPFLAGS += -g -O0 -fsanitize=address,undefined
debug: re