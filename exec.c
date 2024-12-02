/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoubbi <ekoubbi@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:37:54 by ekoubbi           #+#    #+#             */
/*   Updated: 2024/12/02 07:34:33 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define WRITE 1
#define READ 0

static int	ft_close(int *fd)
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

static void	closetab(int count, int *fd_tab)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_close(&fd_tab[i]);
		fd_tab[i] = -1;
		i++;
	}
}

static void	ft_free(char **tab)
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

// TODO less mallocs?
// TODO test invalid PATH
// FIXME . is in always in path
static char	*get_valid_path(char *cmd, t_env *env)
{
	char	*path_line;
	char	**array_env;
	char	*path;
	int		i;

	if (cmd == NULL)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path_line = env_get(env, "PATH");
	if (path_line == NULL)
		path_line = ".";
	array_env = ft_split(path_line, ':');
	i = 0;
	while (array_env[i])
	{
		path = ft_strjoinv(3, array_env[i], "/", cmd);
		if (access(path, X_OK) == 0)
			return (ft_free(array_env), path);
		free(path);
		i++;
	}
	ft_free(array_env);
	return (NULL);
}

// TODO check error handling
static bool	handle_redirection(t_ctx *ctx, t_cmd *cmd)
{
	int		fd;
	bool	ret;

	ret = true;
	if (cmd->redir[0].filename != NULL)
	{
		fd = open(cmd->redir[0].filename, O_RDONLY);
		if (fd < 0)
			ret = (eno(ctx, E_OPEN), false);
		(dup2(fd, STDIN_FILENO), close(fd));
	}
	if (cmd->redir[1].filename != NULL)
	{
		if (cmd->redir[1].append)
			fd = open(cmd->redir[1].filename,
					O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
		else
			fd = open(cmd->redir[1].filename,
					O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fd < 0)
			ret = (eno(ctx, E_OPEN), false);
		(dup2(fd, STDOUT_FILENO), close(fd));
	}
	return (ret);
}

static void	run_bt(t_ctx *ctx, t_cmd *cmd)
{
	t_bt_f	bt;

	bt = get_builtin(cmd);
	ctx->exitcode = EXIT_SUCCESS;
	if (!bt(ctx, cmd))
	{
		err_p_clear(cmd->argv[0], &ctx->err);
		ctx->exitcode = EXIT_FAILURE;
	}
}

// TODO handle redirect errors
static bool	execute(t_ctx *ctx, t_cmd *cmd)
{
	char	*valid_path;
	char	**env;

	if (!handle_redirection(ctx, cmd))
		return (false);
	if (get_builtin(cmd))
		return (run_bt(ctx, cmd), true);
	valid_path = get_valid_path(cmd->argv[0], ctx->env);
	if (valid_path == NULL)
		return (eno(ctx, E_CMD_NOT_FOUND), ctx->exitcode = 127, false);
	env = env_environ(ctx->env);
	if (!env)
		return (eno(ctx, E_MEM), free(valid_path), false);
	execve(valid_path, cmd->argv, env);
	eno(ctx, E_EXECVE);
	ctx->exitcode = EXIT_FAILURE;
	return (free(valid_path), free(env), false);
}

// TODO correctly handle errors
// yes, you're reading this right, the child returns to the main to exit
static bool	exec_fork(int *fdin, t_cmd *cmd, t_ctx *ctx, bool should_pipe)
{
	int		pipefd[2];

	if (should_pipe && pipe(pipefd) == -1)
		return (eno(ctx, E_PIPE), false);
	cmd->pid = fork();
	if (cmd->pid < 0)
		return (eno(ctx, E_FORK), closetab(2, pipefd), false);
	if (cmd->pid == 0)
	{
		dup2(*fdin, STDIN_FILENO);
		ft_close(fdin);
		if (should_pipe)
		{
			dup2(pipefd[WRITE], STDOUT_FILENO);
			closetab(2, pipefd);
		}
		if (cmd->argc && execute(ctx, cmd))
			eno(ctx, E__NOPRINT);
		return (ctx->should_exit = true, false);
	}
	ft_close(fdin);
	if (should_pipe)
		ft_close(&pipefd[WRITE]);
	return (*fdin = pipefd[READ], true);
}

static bool	exec_builtin_nofork(t_ctx *ctx, t_cmd *cmd)
{
	int	fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (fd[0] < 0 || fd[1] < 0)
		return (closetab(2, fd), false);
	execute(ctx, cmd);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	closetab(2, fd);
	return (true);
}

// TODO error handling!!!!!!
bool	exec_cmds(t_ctx *ctx)
{
	int		i;
	int		fdin;
	int		status;

	if (ctx->cmd_count == 1 && get_builtin(&ctx->cmds[0]))
		return (exec_builtin_nofork(ctx, &ctx->cmds[0]));
	fdin = STDIN_FILENO;
	i = -1;
	while (++i < ctx->cmd_count)
		if (!exec_fork(&fdin, &ctx->cmds[i], ctx, i < ctx->cmd_count - 1))
			return (ft_close(&fdin), false);
	i = 0;
	while (i < ctx->cmd_count)
		waitpid(ctx->cmds[i++].pid, &status, 0);
	ctx->exitcode = WEXITSTATUS(status);
	return (true);
}
