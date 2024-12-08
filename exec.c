/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoubbi <ekoubbi@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:37:54 by ekoubbi           #+#    #+#             */
/*   Updated: 2024/12/08 17:15:34 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define WRITE 1
#define READ 0

static bool	_try_execve_path(t_ctx *ctx, t_cmd *cmd, char **env, int *errsv)
{
	char		**paths;
	int			i;
	char		*command;
	struct stat	buf;

	paths = ft_split(env_get(ctx->env, "PATH"), ':');
	if (paths == NULL)
		return (enosv(ctx, E_MEM, ENOMEM), false);
	i = -1;
	while (paths[++i])
	{
		command = ft_strjoinv(3, paths[i], "/", cmd->argv[0]);
		if (command == NULL)
			return (eno(ctx, E_MEM), free_tab(paths), false);
		if (stat(command, &buf) == 0 && !S_ISDIR(buf.st_mode))
		{
			set_signals(S_DEFAULT);
			execve(command, cmd->argv, env);
			*errsv = errno;
			set_signals(S_IGNORE);
			enosv(ctx, E_EXECVE, *errsv);
		}
		free(command);
	}
	return (free_tab(paths), true);
}

static bool	try_execve(t_ctx *ctx, t_cmd *cmd, char **env)
{
	int			errsv;
	struct stat	buf;

	errsv = ENOENT;
	if (ft_strchr(cmd->argv[0], '/') || env_get(ctx->env, "PATH") == NULL)
	{
		set_signals(S_DEFAULT);
		execve(cmd->argv[0], cmd->argv, env);
		errsv = errno;
		set_signals(S_IGNORE);
		enosv(ctx, E_EXECVE, errsv);
		if (stat(cmd->argv[0], &buf) == 0 && S_ISDIR(buf.st_mode))
			eno(ctx, E_CMD_IS_DIR);
	}
	else
		if (!_try_execve_path(ctx, cmd, env, &errsv))
			return (false);
	if (errsv == ENOENT)
		return (eno(ctx, E_CMD_NOT_FOUND),
			ctx->exitcode = EXIT_NOT_FOUND, false);
	return (ctx->exitcode = EXIT_EXEC_ERROR, false);
}

bool	execute(t_ctx *ctx, t_cmd *cmd)
{
	char	**env;

	if (!handle_redirection(ctx, cmd))
		return (ctx->exitcode = EXIT_FAILURE, false);
	if (get_builtin(cmd))
		return (run_builtin(ctx, cmd), true);
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
		ctx->is_child = true;
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
	if (WIFEXITED(status))
		ctx->exitcode = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ctx->exitcode = EXIT_SIG + WTERMSIG(status);
	else
		ctx->exitcode = EXIT_EXEC_ERROR;
	return (true);
}
