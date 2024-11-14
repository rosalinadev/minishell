/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:57 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/11 14:36:36 by rvandepu         ###   ########.fr       */
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
			return (parser->ctx->eno = E_MEM, false);
		return (parser->count = depth, true);
	}
	if (!parse_tokens(parser, depth + 1))
		return (free(token), false);
	return (parser->tokens[depth] = token, true);
}

static bool	parse_command(t_ctx *ctx, t_cmd *cmd, char **cmdline)
{
	t_parser	parser;

	*cmd = (t_cmd){};
	get_parser(&parser, NULL, T_CMD);
	parser.ctx = ctx;
	parser.redir = cmd->redir;
	parser.cmdline = cmdline;
	if (!parse_tokens(&parser, 0))
		return (free_cmd(cmd), false);
	cmd->argc = parser.count;
	cmd->argv = parser.tokens;
	return (true);
}

int	parse_cmdline(t_ctx *ctx, char *cmdline, int depth)
{
	static bool	was_pipe = false;
	t_cmd		cmd;

	while (ft_in(*cmdline, WHITESPACE))
		cmdline++;
	if (*cmdline == '|')
		return (ctx->eno = E_EXP_CMD_PIPE, false);
	if (!*cmdline)
	{
		if (was_pipe)
			return (ctx->eno = E_PIPE_EXP_CMD, false);
		ctx->cmds = ft_calloc(depth, sizeof(t_cmd));
		if (ctx->cmds == NULL)
			return (ctx->eno = E_MEM, false);
		return (ctx->cmd_count = depth, true);
	}
	if (!parse_command(ctx, &cmd, &cmdline))
		return (false);
	was_pipe = (*cmdline == '|') && (cmdline++, true);
	if (!parse_cmdline(ctx, cmdline, depth + 1))
		return (free_cmd(&cmd), false);
	return (ctx->cmds[depth] = cmd, true);
}
