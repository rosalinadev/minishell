/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:14:24 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/08 21:18:24 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO remove
bool	parse_cmd(char *cmdline);

int	main(void)
{
	char	*cmdline;
	bool	should_exit;

	while (true)
	{
		cmdline = readline("[OhMyPKshell]$ ");
		if (!cmdline)
			return (EXIT_SUCCESS);
		ft_printf("%s\n", cmdline);
		parse_cmd(cmdline);
		should_exit = !strcmp(cmdline, "exit");
		free(cmdline);
		if (should_exit)
			return (EXIT_SUCCESS);
	}
}
