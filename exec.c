/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoubbi <ekoubbi@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:37:54 by ekoubbi           #+#    #+#             */
/*   Updated: 2024/12/03 19:00:36 by rvandepu         ###   ########.fr       */
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
		ft_close(&fd_tab[i++]);
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
		(dup2(fd, STDIN_FILENO), ft_close(&fd));
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
		(dup2(fd, STDOUT_FILENO), ft_close(&fd));
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

static bool	_try_execve_path(t_ctx *ctx, t_cmd *cmd, char **env, int *errsv)
{
	char		**paths;
	int			i;
	char		*command;
	struct stat	buf;

	paths = ft_split(env_get(ctx->env, "PATH"), ':');
	if (paths == NULL)
		return (enosv(ctx, E_MEM, ENOMEM), false);
	i = 0;
	while (paths[i])
	{
		command = ft_strjoinv(3, paths[i], "/", cmd->argv[0]);
		if (command == NULL)
			return (eno(ctx, E_MEM), ft_free(paths), false);
		if (stat(command, &buf) == 0 && !S_ISDIR(buf.st_mode))
		{
			execve(command, cmd->argv, env);
			*errsv = errno;
			eno(ctx, E_EXECVE);
		}
		free(command);
		i++;
	}
	ft_free(paths);
	return (true);
}

bool	try_execve(t_ctx *ctx, t_cmd *cmd, char **env)
{
	int			errsv;
	struct stat	buf;

	errsv = ENOENT;
	if (ft_strchr(cmd->argv[0], '/') || env_get(ctx->env, "PATH") == NULL)
	{
		execve(cmd->argv[0], cmd->argv, env);
		errsv = errno;
		eno(ctx, E_EXECVE);
		if (stat(cmd->argv[0], &buf) == 0 && S_ISDIR(buf.st_mode))
			eno(ctx, E_CMD_IS_DIR);
	}
	else
		if (!_try_execve_path(ctx, cmd, env, &errsv))
			return (false);
	if (errsv == ENOENT)
		return (eno(ctx, E_CMD_NOT_FOUND), ctx->exitcode = 127, false);
	return (ctx->exitcode = 126, false);
}

static bool	execute(t_ctx *ctx, t_cmd *cmd)
{
	char	**env;

	if (!handle_redirection(ctx, cmd))
		return (ctx->exitcode = EXIT_FAILURE, false);
	if (get_builtin(cmd))
		return (run_bt(ctx, cmd), true);
	env = env_environ(ctx->env);
	if (!env)
		return (eno(ctx, E_MEM), false);
	try_execve(ctx, cmd, env);
	ft_fprintf(stderr, "%s: %s: ", SHELL_NAME, cmd->argv[0]);
	err_p_clear(NULL, &ctx->err);
	eno(ctx, E__NOPRINT);
	return (free(env), false);
}

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
		if (!cmd->argc || execute(ctx, cmd))
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
	int		fd[2];
	bool	ret;

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (fd[0] < 0 || fd[1] < 0)
		return (closetab(2, fd), false);
	ret = execute(ctx, cmd);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	closetab(2, fd);
	return (ret);
}

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
