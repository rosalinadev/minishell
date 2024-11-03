/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   .fuzzer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:16:39 by aboyreau          #+#    #+#             */
/*   Updated: 2024/11/03 13:16:39 by aboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdint.h>

void	print_cmds(t_cmd **cmds)
{
	int		i;
	int		j;

	i = 0;
	while (cmds[i])
	{
		ft_printf("====================\n%10d\n====================\n", i);
		ft_printf("argc: %d\nargv:\n", cmds[i]->argc);
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

void	handle_cmds(t_ctx *ctx)
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


#define PERFECT 42
int	LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	char	*cmdline;
	t_ctx	ctx;

	ctx = (t_ctx){.exitcode = PERFECT};
	env_init(&ctx.env, environ);

	cmdline = calloc(size + 1, sizeof(char));
	size_t i;
	for (i = 0; i < size; i++)
	{
		cmdline[i] = (char) data[i];
	}
	cmdline[i] = '\0';
	if (!cmdline)
		return (true);
	// should_exit = !ft_strcmp(cmdline, "exit");
	add_history(cmdline);
	if (!parse_cmdline(&ctx, cmdline, 0))
	{
		ft_printf("error during parsing\n");
		env_clear(&ctx.env);
		free(cmdline);
		return EXIT_SUCCESS;
	}
	free_cmds(ctx.cmds);
	env_clear(&ctx.env);
	free(cmdline);
	return EXIT_SUCCESS;
}
