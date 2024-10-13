/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:59:38 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/13 07:00:23 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include "libft.h"

typedef enum e_redirtype
{
	R_UNKNOWN = 0,
	R_INPUT,
	R_HEREDOC,
	R_OUTPUT,
	R_APPEND,
}	t_redirtype;

typedef struct s_redir
{
	t_redirtype	type;
	char		*filename;
}	t_redir;

typedef struct s_cmd
{
	t_redir	redir[2];
	int		argc;
	char	**argv;
}	t_cmd;

typedef enum e_token
{
	T_INVALID = 0,
	T_CMD,
	T_DEFAULT,
	T_QUOTE,
	T_DOUBLEQUOTE,
	T_DOLLAR,
	T__MAX,
}	t_token;

typedef struct s_parser
{
	struct s_parser	*parent;
	t_redir			*redir;
	char			**cmdline;
	t_token			type;
	int				count;
	char			**tokens;
}	t_parser;

// parser.c
bool	parse_tokens(t_parser *parser, int depth);

// parser_utils.c
void	get_parser(t_parser *parser, t_parser *parent, t_token type);
void	free_parser(t_parser *parser);
bool	sub_parser(t_parser *parent, t_token type, char **token);

// parse_utils.c
bool	is_whitespace(char c);
char	*dup_token(char *src, int len);

#endif
