/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:56:47 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/04 13:43:29 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	int_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signals(t_setsig set)
{
	if (set == S_INTERACTIVE)
	{
		sigaction(SIGINT, &(struct sigaction){.sa_handler = int_handler}, NULL);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
	}
	else if (set == S_IGNORE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
	}
	else if (set == S_DEFAULT)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
	}
}
