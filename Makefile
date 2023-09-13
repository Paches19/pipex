# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/01 10:46:27 by adpachec          #+#    #+#              #
#    Updated: 2023/09/13 15:48:24 by adpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= pipex

SRC_DIR			= srcs/
INC_DIR			= ./include/
OBJ_DIR			= obj/

SRC				= pipex.c pipex_utils.c matrix_utils.c get_paths_access.c \
					error_management.c

SRCS			= $(addprefix $(SRC_DIR), $(SRC))

OBJ				= $(SRC:.c=.o)
OBJS			= $(addprefix $(OBJ_DIR), $(OBJ))

CC				= gcc
CFLAGS			= -Wall -Wextra -Werror -g

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean:
		rm -rf $(OBJ_DIR)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean