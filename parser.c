/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:57 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/17 17:32:48 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_token(t_parser *parser, char **token)
{
	static bool	(*lookup[T__MAX])(t_parser *, char **) = {\
		[T_CMD] = parse_cmd, \
		[T_DEFAULT] = parse_default, \
		[T_DOUBLEQUOTE] = parse_doublequote, \
	};

	if (!lookup[parser->type])
		return (false);
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

// TODO check this function's return
bool	parse_cmdline(t_cmd **ret, char *cmdline)
{
	t_cmd		cmd;
	t_parser	parser;

	cmd = (t_cmd){};
	get_parser(&parser, NULL, T_CMD);
	parser.in = &cmd.in;
	parser.out = &cmd.out;
	parser.cmdline = &cmdline;
	if (!parse_tokens(&parser, 0))
		return (false);
	cmd.argc = parser.count;
	cmd.argv = parser.tokens;
	*ret = malloc(sizeof(**ret));
	if (*ret == NULL)
		return (false);
	**ret = cmd;
	return (true);
}

/*int	parse_cmdline(void)
{
}*/
