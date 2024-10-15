# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/06 16:11:02 by rvandepu          #+#    #+#              #
#    Updated: 2024/10/15 09:39:13 by rvandepu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME  := minishell
FILES := $(NAME).c \
		 parser.c \
		 parser_utils.c \
		 parse_cmd.c \
		 parse_default.c \
		 parse_var.c \
		 parse_doublequote.c \
		 parse_utils.c \

OBJ := $(FILES:%.c=%.o)

CFLAGS += -Wall -Wextra -Werror

# https://github.com/rosalinadev/libft
LIBFT_DIR	:= libft
LIBFT		:= libft.a
LIBFT_PATH	:= $(LIBFT_DIR)/$(LIBFT)
CPPFLAGS	+= -I$(LIBFT_DIR)
LDFLAGS		+= -L$(LIBFT_DIR)
LDLIBS		+= -lft

# readline
LDLIBS += -lreadline

.PHONY: all clean fclean re test

all: $(NAME)

clean:
	$(MAKE) -s -C $(LIBFT_DIR) fclean
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: CFLAGS += -g -Wno-error
test: re

$(LIBFT_PATH):
	$(MAKE) -s -C $(LIBFT_DIR) $(LIBFT) -j $$(nproc) CFLAGS="$(CFLAGS)"

$(NAME): $(OBJ) | $(LIBFT_PATH)
