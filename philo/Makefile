# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yilin <yilin@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 14:47:59 by yilin             #+#    #+#              #
#    Updated: 2025/01/22 12:37:55 by yilin            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 NAME / MAIN                                  #
# **************************************************************************** #

NAME = philo

# **************************************************************************** #
#                                 DIRECTORIES                                  #
# **************************************************************************** #

INCL_DIR = includes
SRCS_DIR = srcs
OBJS_DIR = objs

FL = cleanup.c  init.c  main.c  routine.c  supervisor.c  tools.c tools2.c

# **************************************************************************** #
#                              SRC & OBJ FILES                                 #
# **************************************************************************** #

# CANNOT ADDPREFIX (MAYBE)
SRCS = $(addprefix $(SRCS_DIR)/, $(FL))
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

DEPS = $(OBJS:.o=.d)

# **************************************************************************** #
#                            COMPILER & FLAGS                                  #
# **************************************************************************** #

CC = cc
#CFLAGS = -Wall -Wextra -Werror -g3 -I$(INCL_DIR)
CFLAGS = -Wall -Wextra -Werror -g3 -I$(INCL_DIR) -pthread -MMD -MP #-fsanitize=thread # Add for linking <pthread.h>
#(-fsanitize=thread doesn't work on codespace)
#-MMD -MP
# LDFLAGS = -L/path/to/libtsan -ltsan #testing for threading flag

RM = rm -rf

# **************************************************************************** #
#                               BUILD COMMANDS                                 #
# **************************************************************************** #

### Build Targets ###
all: $(NAME)

### NAME Create the executable ###
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(ROSEB)🚀🦄 $(NAME) SUCCESSFULLY COMPILED! 🦄🚀$(COLOR_RESET)"

### Compile object files ###
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREENB)---🔧 COMPILING $< TO $@ 🔧---$(COLOR_RESET)"

### Create object directory ###
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

### Clean up object files and directories ###
clean:
	@$(RM) $(OBJS_DIR)
	@echo "$(BABEBLUEB)🧹 CLEAN DONE! OBJS FILES REMOVED 🧹$(COLOR_RESET)"

### Remove all built files ###
fclean: clean
	@$(RM) $(NAME)
	@echo "$(BABEBLUEB)🫧 FCLEAN DONE! [ $(NAME) ] REMOVED 🫧$(COLOR_RESET)"

### Rebuild the project ###
re: fclean all
	@echo "$(REDB)RE DONE$(COLOR_RESET)"
	
# Include dependency files
-include $(DEPS)

# **************************************************************************** #
#                              PHONY TARGETS                                   #
# **************************************************************************** #

.PHONY: all clean fclean re

# **************************************************************************** #
#                              COLOR SETTING                                   #
# **************************************************************************** #

COLOR_RESET = \033[0m
PINKB = \033[1;95m
REDB = \033[1;91m
ROSEB = \033[1;38;5;225m
BLUEB = \033[1;34m
BABEBLUEB = \033[1;38;5;153m
GREENB = \033[1;38;5;85m
PURPLEB = \033[1;38;5;55m
PSTL_YELLOWB = \033[1;38;2;255;253;208m
PSTL_ORGB = \033[1;38;2;255;179;102m
PSTL_PURPLEB =\033[1;38;2;204;153;255m

GREEN_BBG = \033[1;42m
BLUE_BBG = \033[1;44m
YELLOW_BBG = \033[1;43m
ORANGE_BBG = \033[1;48;5;214m
