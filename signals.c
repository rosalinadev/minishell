/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:56:47 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/16 20:27:28 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	int_handler(int sig)
{
	(void)sig;
	ft_fprintf(rl_outstream, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	g_last_sig = 0;

static void	save_sig(int sig)
{
	g_last_sig = sig;
}

static void	ignore(int sig)
{
	(void)sig;
}

void	set_signals(t_setsig set)
{
	static bool	has_init = false;

	if (!has_init)
	{
		sigaction(SIGQUIT, &(struct sigaction){.sa_handler = ignore}, NULL);
		sigaction(SIGPIPE, &(struct sigaction){.sa_handler = ignore}, NULL);
		has_init = true;
	}
	if (set == S_INTERACTIVE)
		sigaction(SIGINT, &(struct sigaction){.sa_handler = int_handler}, NULL);
	else if (set == S_HEREDOC)
		sigaction(SIGINT, &(struct sigaction){.sa_handler = save_sig}, NULL);
	else if (set == S_IGNORE)
		sigaction(SIGINT, &(struct sigaction){.sa_handler = ignore}, NULL);
}
