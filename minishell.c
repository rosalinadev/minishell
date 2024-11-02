/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/02 02:09:59 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_cmds(t_cmd **cmds)
{
	int		i;
	int		j;

	i = 0;
	while (cmds[i])
	{
		ft_printf("====================\n%10d\n====================\n", i);
		if (cmds[i]->argc == 0)
			ft_printf("(no arguments to print)\n");
		j = 0;
		while (j < cmds[i]->argc)
		{
			ft_printf("%3d: '%s'\n", j, cmds[i]->argv[j]);
			j++;
		}
		ft_printf("in: %s\n\tis_heredoc:%d\n\tquoted:%d\n",
			cmds[i]->redir[0].filename, cmds[i]->redir[0].is_heredoc,
			cmds[i]->redir[0].quoted);
		ft_printf("out: %s\n\tappend:%d\n", cmds[i]->redir[1].filename,
			cmds[i]->redir[1].append);
		i++;
	}
}

static void	handle_cmds(t_ctx *ctx)
{
	t_cmd	*cmd;
	t_env	*var;
	char	*str;

	cmd = *ctx->cmds;
	if (cmd == NULL)
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
			free(cmd->argv[1]);
			cmd->argv[1] = str;
		}
		if (!env_set(&ctx->env, cmd->argv[1], false))
			return (env_clear(&ctx->env));
	}
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
	{
		if (cmd->argc < 2)
			return ;
		env_set(&ctx->env, cmd->argv[1], true);
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
			return ; // TODO handle
		int	i = 0;
		while (env[i])
		{
			ft_printf("  %d: %s\n", i, env[i]);
			i++;
		}
		free(env);
	}
}

static void	free_cmds(t_cmd **cmds)
{
	int		i;

	i = 0;
	while (cmds[i])
	{
		free_cmd(cmds[i++], true);
	}
	free(cmds);
}

static bool	loop(t_ctx *ctx)
{
	char	*cmdline;
	bool	should_exit;

	while (true)
	{
		cmdline = readline("[OhMyPKshell]$ ");
		if (!cmdline)
			return (true);
		should_exit = !ft_strcmp(cmdline, "exit");
		ft_printf("%s\n", cmdline);
		add_history(cmdline);
		if (!parse_cmdline(ctx, cmdline, 0))
		{
			ft_printf("error during parsing\n");
			free(cmdline);
			continue ;
		}
		free(cmdline);
		print_cmds(ctx->cmds);
		handle_cmds(ctx);
		free_cmds(ctx->cmds);
		if (should_exit)
			return (true);
	}
}

int	main(void)
{
	t_ctx	ctx;

	ctx = (t_ctx){.exitcode = 42};
	env_init(&ctx.env, environ);
	if (!loop(&ctx))
		return (env_clear(&ctx.env), EXIT_FAILURE);
	return (env_clear(&ctx.env), EXIT_SUCCESS);
}
