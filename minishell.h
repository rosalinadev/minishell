/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:59:38 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/17 23:14:52 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define WHITESPACE " \t"

typedef struct s_r_in
{
	char	*filename;
	bool	is_heredoc;
	bool	hd_expand;
	bool	hd_strip;
}	t_r_in;

typedef struct s_r_out
{
	char	*filename;
	bool	append;
}	t_r_out;

typedef struct s_cmd
{
	t_r_in	in;
	t_r_out	out;
	int		argc;
	char	**argv;
}	t_cmd;

typedef enum e_token
{
	T_INVALID = 0,
	T_CMD,
	T_DEFAULT,
	T_DOUBLEQUOTE,
	T__MAX,
}	t_token;

typedef struct s_parser
{
	t_r_in	*in;
	t_r_out	*out;
	char	**cmdline;
	t_token	type;
	int		count;
	char	**tokens;
}	t_parser;

typedef enum e_redirect
{
	R_IN,
	R_HEREDOC,
	R_HD_TRIM,
	R_OUT,
	R_APPEND
}	t_redirect;

// parser.c
bool	parse_token(t_parser *parser, char **token);
bool	parse_tokens(t_parser *parser, int depth);
bool	parse_cmdline(t_cmd **ret, char *cmdline);

// parser_utils.c
void	get_parser(t_parser *parser, t_parser *parent, t_token type);
void	free_parser(t_parser *parser);
bool	sub_parser(t_parser *parent, t_token type, char **token);

// parse_cmd.c
bool	parse_cmd(t_parser *parser, char **token);

// parse_default.c
bool	parse_default(t_parser *parser, char **token);

// parse_var.c
bool	parse_var(t_parser *parser, char **token);

// parse_doublequote.c
bool	parse_doublequote(t_parser *parser, char **token);

// parse_utils.c
char	*dup_token(char *src, int len);

#endif
