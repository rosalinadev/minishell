/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/19 16:00:29 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->argc)
	{
		ft_printf("%3d: '%s'\n", i, cmd->argv[i]);
		free(cmd->argv[i++]);
	}
	free(cmd->argv);
	ft_printf("in: %s\n\tis_heredoc:%d\n\tquoted:%d\n", cmd->redir[0].filename,
		cmd->redir[0].is_heredoc, cmd->redir[0].quoted);
	ft_printf("out: %s\n\tappend:%d\n", cmd->redir[1].filename,
		cmd->redir[1].append);
	free(cmd->redir[0].filename);
	free(cmd->redir[1].filename);
	free(cmd);
}

int	main(void)
{
	char	*cmdline;
	bool	should_exit;
	t_cmd	*cmd;

	while (true)
	{
		cmdline = readline("[OhMyPKshell]$ ");
		//cmdline = get_next_line(0);
		if (!cmdline)
			return (EXIT_SUCCESS);
		///*ft_strchr(cmdline, '\n') = '\0';
		should_exit = !ft_strcmp(cmdline, "exit");
		ft_printf("%s\n", cmdline);
		//ft_printf("[OhMyPKshell]$ %s\n", cmdline);
		add_history(cmdline);
		if (!parse_cmdline(&cmd, cmdline))
		{
			ft_printf("error during parsing\n");
			free(cmdline);
			continue ;
		}
		free(cmdline);
		//cmdline = ft_strjoin(cmd->argc, cmd->argv);
		print_free_cmd(cmd);
		//ft_printf("%s\n", cmdline);
		//free(cmdline);
		if (should_exit)
			return (EXIT_SUCCESS);
	}
}
