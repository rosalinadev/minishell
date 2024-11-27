/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:32:56 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/27 03:36:30 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_opt_n(char *str)
{
	int	i;

	if (str[0] == '-')
	{
		i = 1;
		if (str[i] == 'n')
		{
			while (str[i] == 'n')
				i++;
			if (str[i] == '\0')
				return (true);
		}
	}
	return (false);
}

// TODO echo -n -n -n jtm chayanne
bool	bt_echo(t_ctx *ctx, t_cmd *cmd)
{
	int		i;
	bool	opt_no_nl;

	(void)ctx;
	opt_no_nl = false;
	if (cmd->argc >= 2)
		opt_no_nl = has_opt_n(cmd->argv[1]);
	i = 1 + opt_no_nl;
	while (i < cmd->argc)
	{
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		i++;
		if (i < cmd->argc)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!opt_no_nl)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (true);
}
