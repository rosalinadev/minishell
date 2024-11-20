/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/19 05:27:09 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_cmds(t_ctx *ctx)
{
	int		i;
	int		j;

	i = 0;
	while (i < ctx->cmd_count)
	{
		ft_printf("====================\n%10d\n====================\n", i);
		ft_printf("argc: %d\nargv:\n", ctx->cmds[i].argc);
		if (ctx->cmds[i].argc == 0)
			ft_printf("(no arguments to print)\n");
		j = 0;
		while (j < ctx->cmds[i].argc)
		{
			ft_printf("%3d: '%s'\n", j, ctx->cmds[i].argv[j]);
			j++;
		}
		ft_printf("in: %s\n\tis_heredoc:%d\n\tquoted:%d\n",
			ctx->cmds[i].redir[0].filename, ctx->cmds[i].redir[0].is_heredoc,
			ctx->cmds[i].redir[0].quoted);
		ft_printf("out: %s\n\tappend:%d\n", ctx->cmds[i].redir[1].filename,
			ctx->cmds[i].redir[1].append);
		i++;
	}
}

/*static void	handle_cmds(t_ctx *ctx)
{
	t_cmd	*cmd;
	t_env	*var;
	char	*str;

	cmd = *ctx->cmds;
	if (cmd == NULL || !cmd->argc)
		return ;
	if (ft_strcmp(cmd->argv[0], "export") == 0)
	{
		if (cmd->argc < 2)
			return ;
		if (!ft_strchr(cmd->argv[1], '='))
		{
			str = env_get(ctx->env, cmd->argv[1]);
			ft_printf("[export] str: %p %d\n", str, str == NULL);
			str = ft_strjoinv(3 - (str == NULL), cmd->argv[1], "=", str);
			if (str == NULL)
				return ;
			free(cmd->argv[1]);
			cmd->argv[1] = str;
		}
		if (!env_set(&ctx->env, cmd->argv[1]))
			return (env_clear(&ctx->env));
	}
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
	{
		if (cmd->argc < 2)
			return ;
		env_del(&ctx->env, cmd->argv[1]);
	}
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
	{
		var = ctx->env;
		while (var)
		{
			ft_printf("%s\n", var->var);
			var = var->next;
		}
	}
	else if (ft_strcmp(cmd->argv[0], "environ") == 0)
	{
		char	**env = env_environ(ctx->env);
		ft_printf("[environ] env: %p\n", env);
		if (!env)
			return ; // TODO malloc fail
		int	i = 0;
		while (env[i])
		{
			ft_printf("  %d: %s\n", i, env[i]);
			i++;
		}
		free(env);
	}
}*/

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
	//print_cmds(ctx);
	if (ctx->cmd_count)
		exec_cmds(ctx);
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
	return (ctx.exitcode);
}
