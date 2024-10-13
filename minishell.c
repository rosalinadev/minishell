/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/13 04:01:40 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO remove
extern t_cmd	*parse_cmdline(char *cmdline);

static void	print_cmd(t_cmd *cmd)
{
	int	i;

	i = -1;
	while (++i < cmd->argc)
		printf("%3d: '%s'\n", i, cmd->argv[i]);
}

int	main(void)
{
	char	*cmdline;
	bool	should_exit;
	t_cmd	*cmd;

	while (true)
	{
		cmdline = readline("[OhMyPKshell]$ ");
		if (!cmdline)
			return (EXIT_SUCCESS);
		ft_printf("%s\n", cmdline);
		cmd = parse_cmdline(cmdline);
		print_cmd(cmd);
		ft_printf("%s\n", ft_strjoin(cmd->argc, cmd->argv));
		should_exit = !strcmp(cmdline, "exit");
		free(cmdline);
		if (should_exit)
			return (EXIT_SUCCESS);
	}
}
