/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 17:11:33 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/08 17:15:09 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_close(int *fd)
{
	int	ret;

	ret = 0;
	if (*fd >= 0 && *fd != STDIN_FILENO
		&& *fd != STDOUT_FILENO && *fd != STDERR_FILENO)
	{
		ret = close(*fd);
		*fd = -1;
	}
	return (ret);
}

void	closetab(int count, int *fd_tab)
{
	int	i;

	i = 0;
	while (i < count)
		ft_close(&fd_tab[i++]);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

bool	handle_redirection(t_ctx *ctx, t_cmd *cmd)
{
	int		fd;
	bool	ret;

	ret = true;
	if (cmd->redir[0].filename != NULL)
	{
		fd = open(cmd->redir[0].filename, O_RDONLY);
		if (fd < 0)
			ret = (eno(ctx, E_OPEN), false);
		(dup2(fd, STDIN_FILENO), ft_close(&fd));
	}
	if (cmd->redir[1].filename != NULL)
	{
		if (cmd->redir[1].append)
			fd = open(cmd->redir[1].filename, O_WRONLY | O_CREAT | O_APPEND,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		else
			fd = open(cmd->redir[1].filename, O_WRONLY | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		if (fd < 0)
			ret = (eno(ctx, E_OPEN), false);
		(dup2(fd, STDOUT_FILENO), ft_close(&fd));
	}
	return (ret);
}
