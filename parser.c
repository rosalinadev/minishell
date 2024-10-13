/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:57 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/13 03:55:58 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	invalid_parser(t_parser *parser, char **token) {return (false);}
bool	parse_cmd(t_parser *parser, char **token);
bool	parse_default(t_parser *parser, char **token);
bool	parse_quote(t_parser *parser, char **token) {return (false);}
bool	parse_doublequote(t_parser *parser, char **token) {return (false);}
bool	parse_dollar(t_parser *parser, char **token) {return (false);}

bool	parse_token(t_parser *parser, char **token)
{
	static bool	(*lookup[T__MAX])(t_parser *, char **) = {\
		[T_INVALID] = invalid_parser, \
		[T_CMD] = parse_cmd, \
		[T_DEFAULT] = parse_default, \
		[T_QUOTE] = parse_quote, \
		[T_DOUBLEQUOTE] = parse_doublequote, \
		[T_DOLLAR] = parse_dollar, \
	};

	return (lookup[parser->type](parser, token));
}

bool	parse_tokens(t_parser *parser, int depth)
{
	char	*token;

	token = NULL;
	if (!parse_token(parser, &token))
		return (false);
	if (token == NULL)
	{
		parser->tokens = ft_calloc(depth + 1, sizeof(char *));
		if (parser->tokens == NULL)
			return (false);
		return (parser->count = depth, true);
	}
	if (!parse_tokens(parser, depth + 1))
		return (free(token), false);
	return (parser->tokens[depth] = token, true);
}

t_cmd	*parse_cmdline(char *cmdline)
{
	t_cmd		cmd;
	t_parser	parser;
	t_cmd		*ret;

	cmd = (t_cmd){};
	get_parser(&parser, NULL, T_CMD);
	parser.redir = cmd.redir;
	parser.cmdline = &cmdline;
	if (!parse_tokens(&parser, 0))
		return (NULL);
	cmd.argc = parser.count;
	cmd.argv = parser.tokens;
	ret = malloc(sizeof(*ret));
	if (ret)
		*ret = cmd;
	return (ret);
}

/*int	parse_cmdline(void)
{
}*/
