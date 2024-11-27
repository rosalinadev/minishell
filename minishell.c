/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/26 20:14:38 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmds(t_ctx *ctx)
{
	while (ctx->cmd_count--)
		free_cmd(&ctx->cmds[ctx->cmd_count]);
	free(ctx->cmds);
	ctx->cmds = NULL;
}

// TODO signals
// TODO heredocs
// TODO exit
static bool	loop(t_ctx *ctx)
{
	char	*prompt;
	char	*cmdline;

	prompt = env_get(ctx->env, "PS1");
	if (!prompt)
		prompt = "$ ";
	cmdline = readline(prompt);
	if (!cmdline)
		return (ctx->should_exit = true);
	add_history(cmdline);
	if (!parse_cmdline(ctx, cmdline, 0))
	{
		err_p_clear("error during parsing", &ctx->eno);
		free(cmdline);
		return (true);
	}
	free(cmdline);
	if (ctx->debug_hook)
		ctx->debug_hook(ctx);
	if (ctx->cmd_count)
		if (!exec_cmds(ctx))
			err_p_clear("error during exec", &ctx->eno);
	free_cmds(ctx);
	return (true);
}

// TODO
int	main(void)
{
	t_ctx	ctx;

	rl_outstream = stderr;
	ctx = (t_ctx){.exitcode = EXIT_SUCCESS};
	if (!env_init(&ctx.env, environ))
		return (err_p("main", E_MEM), EXIT_FAILURE);
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
