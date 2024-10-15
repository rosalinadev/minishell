/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:57 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/15 10:41:02 by rvandepu         ###   ########.fr       */
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
