/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/22 20:53:54 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_free_cmds(t_cmd **cmds)
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
		free_cmd(cmds[i], true);
		i++;
	}
	free(cmds);
}

int	main(void)
{
	char	*cmdline;
	bool	should_exit;
	t_cmd	**cmds;

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
		if (!parse_cmdline(&cmds, cmdline, 0))
		{
			ft_printf("error during parsing\n");
			free(cmdline);
			continue ;
		}
		free(cmdline);
		//cmdline = ft_strjoin(cmd->argc, cmd->argv);
		print_free_cmds(cmds);
		//ft_printf("%s\n", cmdline);
		//free(cmdline);
		if (should_exit)
			return (EXIT_SUCCESS);
	}
}
