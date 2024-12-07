/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/07 22:42:16 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmds(t_ctx *ctx)
{
	while (ctx->cmd_count--)
	{
		if (!ctx->is_child && ctx->cmds[ctx->cmd_count].redir[0].is_heredoc)
			unlink(ctx->cmds[ctx->cmd_count].redir[0].filename);
		free_cmd(&ctx->cmds[ctx->cmd_count]);
	}
	free(ctx->cmds);
	ctx->cmds = NULL;
}

// TODO signals
// TODO heredocs
static bool	loop(t_ctx *ctx)
{
	char	*prompt;
	char	*cmdline;

	prompt = env_get(ctx->env, "PS1");
	if (!prompt)
		prompt = "$ ";
	set_signals(S_INTERACTIVE);
	cmdline = readline(prompt);
	set_signals(S_IGNORE);
	if (!cmdline)
		return (ctx->should_exit = true);
	add_history(cmdline);
	if (!parse_cmdline(ctx, cmdline, 0))
		return (err_p_clear(SHELL_NAME": error during parsing", &ctx->err),
			free(cmdline), true);
	free(cmdline);
	if (ctx->cmd_count && !read_heredocs(ctx))
		return (err_p_clear(SHELL_NAME": error reading heredocs", &ctx->err),
			free_cmds(ctx), true);
	if (ctx->debug_hook)
		ctx->debug_hook(ctx);
	if (ctx->cmd_count && !exec_cmds(ctx))
		err_p_clear(SHELL_NAME": error during exec", &ctx->err);
	free_cmds(ctx);
	return (true);
}

extern char	**environ;

// TODO idk finish the project
int	main(void)
{
	t_ctx	ctx;

	rl_outstream = stderr;
	ctx = (t_ctx){.exitcode = EXIT_SUCCESS};
	if (!env_init(&ctx, environ))
		return (err_p(SHELL_NAME": error during init", &ctx.err), EXIT_FAILURE);
	while (!ctx.should_exit)
	{
		if (!loop(&ctx))
		{
			ctx.exitcode = EXIT_FAILURE;
			break ;
		}
	}
	env_clear(&ctx.env);
	rl_clear_history();
	return (ctx.exitcode);
}
