/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/15 09:53:54 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO remove
extern t_cmd	*parse_cmdline(char *cmdline);

static void	print_free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->argc)
	{
		printf("%3d: '%s'\n", i, cmd->argv[i]);
		free(cmd->argv[i++]);
	}
	free(cmd->argv);
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
		should_exit = !strcmp(cmdline, "exit");
		ft_printf("%s\n", cmdline);
		//ft_printf("[OhMyPKshell]$ %s\n", cmdline);
		cmd = parse_cmdline(cmdline);
		add_history(cmdline);
		free(cmdline);
		if (cmd == NULL)
		{
			ft_printf("error during parsing\n");
			continue ;
		}
		//cmdline = ft_strjoin(cmd->argc, cmd->argv);
		print_free_cmd(cmd);
		//ft_printf("%s\n", cmdline);
		//free(cmdline);
		if (should_exit)
			return (EXIT_SUCCESS);
	}
}
