# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/06 16:11:02 by rvandepu          #+#    #+#              #
#    Updated: 2024/11/03 13:39:03 by aboyreau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME  := minishell
FILES := $(NAME).c \
		 env.c \
		 env_utils.c \
		 parser.c \
		 parser_utils.c \
		 parse_cmd.c \
		 parse_default.c \
		 parse_var.c \
		 parse_doublequote.c \

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

fuzzer: CC = clang
fuzzer: CFLAGS += -O1 -fsanitize=fuzzer,address
fuzzer: OBJ := $(filter-out minishell.o,$(OBJ))
fuzzer: LDLIBS += -fsanitize=fuzzer,address
fuzzer: fclean $(OBJ)
	rm -f $(NAME).o
	$(CC) $(CFLAGS) -c $(CPPFLAGS) .fuzzer.c -o .fuzzer.o
	$(MAKE) -C libft -j 16
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) .fuzzer.o -o $(NAME) $(LDLIBS)
	$(MAKE) clean


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
