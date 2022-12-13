# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/13 16:59:16 by adpachec          #+#    #+#              #
#    Updated: 2022/12/13 17:00:02 by adpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= pipex

SRC_DIR			= srcs/
INC_DIR			= ./include/
OBJ_DIR			= obj/

SRC				= pipex.c pipex_utils_1.c pipex_utils_2.c pipex_utils_3.c \
					pipex_get_fts.c

SRCS			= $(addprefix $(SRC_DIR), $(SRC))

OBJ				= $(SRC:.c=.o)
OBJS			= $(addprefix $(OBJ_DIR), $(OBJ))

CC				= gcc
CFLAGS			= -Wall -Wextra -Werror

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