/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:57 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/21 18:59:12 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// has_skipped gets set by the called parser when it skips a token (redir, etc)
static bool	parse_token(t_parser *parser, char **token)
{
	static bool	(*lookup[T__MAX])(t_parser *, char **) = {\
		[T_CMD] = parse_cmd, \
		[T_DEFAULT] = parse_default, \
		[T_DOUBLEQUOTE] = parse_doublequote, \
	};

	if (!lookup[parser->type])
		return (false);
	parser->has_skipped = true;
	while (parser->has_skipped)
	{
		parser->has_skipped = false;
		if (!lookup[parser->type](parser, token))
			return (false);
	}
	return (true);
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
static bool	parse_command(t_cmd **ret, char **cmdline)
{
	t_cmd		cmd;
	t_parser	parser;

	cmd = (t_cmd){};
	get_parser(&parser, NULL, T_CMD);
	parser.redir = cmd.redir;
	parser.cmdline = cmdline;
	if (!parse_tokens(&parser, 0))
		return (free_cmd(&cmd, false), false);
	cmd.argc = parser.count;
	cmd.argv = parser.tokens;
	*ret = malloc(sizeof(**ret));
	if (*ret == NULL)
		return (free_cmd(&cmd, false), false);
	**ret = cmd;
	return (true);
}

int	parse_cmdline(t_cmd ***ret, char *cmdline, int depth)
{
	static bool	was_pipe = false;
	t_cmd		*cmd;

	ft_printf("[parse_cmdline] d:%d cmd:'%s'\n", depth, cmdline);
	while (ft_in(*cmdline, WHITESPACE))
		cmdline++;
	if (*cmdline == '|')
		return (false);
	if (!*cmdline)
	{
		if (was_pipe)
			return (false);
		*ret = ft_calloc(depth + 1, sizeof(t_cmd *));
		if (*ret == NULL)
			return (false);
		return (true);
	}
	if (!parse_command(&cmd, &cmdline))
		return (false);
	was_pipe = (*cmdline == '|') && (cmdline++, true);
	if (!parse_cmdline(ret, cmdline, depth + 1))
		return (free_cmd(cmd, true), false);
	return ((*ret)[depth] = cmd, true);
}
