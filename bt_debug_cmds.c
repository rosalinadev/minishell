/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_debug_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 08:12:06 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/27 04:14:25 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	debug_print_cmds(t_ctx *ctx)
{
	int		i;
	int		j;

	i = 0;
	while (i < ctx->cmd_count)
	{
		ft_fprintf(stderr, "====================\n%10d\n====================\n"
			"argc: %d\nargv:\n", i, ctx->cmds[i].argc);
		if (ctx->cmds[i].argc == 0)
			ft_fprintf(stderr, "(no arguments to print)\n");
		j = 0;
		while (j < ctx->cmds[i].argc)
		{
			ft_fprintf(stderr, "%3d: '%s'\n", j, ctx->cmds[i].argv[j]);
			j++;
		}
		ft_fprintf(stderr, "in: %s\n\tis_heredoc:%d\n\tquoted:%d\n",
			ctx->cmds[i].redir[0].filename, ctx->cmds[i].redir[0].is_heredoc,
			ctx->cmds[i].redir[0].quoted);
		ft_fprintf(stderr, "out: %s\n\tappend:%d\n",
			ctx->cmds[i].redir[1].filename, ctx->cmds[i].redir[1].append);
		i++;
	}
}

#define PREFIX "[debug:cmds] "

bool	bt_dbg_cmds(t_ctx *ctx, t_cmd *cmd)
{
	(void)cmd;
	if (ctx->debug_hook != debug_print_cmds)
	{
		ft_fprintf(stderr, PREFIX "setting hook to print_cmds\n");
		ctx->debug_hook = debug_print_cmds;
	}
	else
	{
		ft_fprintf(stderr, PREFIX "clearing hook\n");
		ctx->debug_hook = NULL;
	}
	return (true);
}
