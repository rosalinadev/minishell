/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekoubbi <ekoubbi@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:37:54 by ekoubbi           #+#    #+#             */
/*   Updated: 2024/11/14 03:56:34 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define WRITE 1
#define READ 0

static int	ft_close(int fd)
{
	if (fd > 2)
		return (close(fd));
	return (0);
}

static void	closepipe(int *pipefd)
{
	ft_close(pipefd[READ]);
	ft_close(pipefd[WRITE]);
}

static void	ft_free(char **tab)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

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
		{
			ft_free(array_env);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free(array_env);
	return (NULL);
}

static bool	handle_redirection(t_ctx *ctx, t_cmd *cmd)
{
	int	fd1;
	int	fd2;

	if (cmd->redir[0].filename != NULL)
	{
		fd1 = open(cmd->redir[0].filename, O_RDONLY);
		if (fd1 < 0)
			return (ctx->eno = E_OPEN, false);
		dup2(fd1, STDIN_FILENO);
	}
	if (cmd->redir[1].filename != NULL)
	{
		if (cmd->redir[1].append)
			fd2 = open(cmd->redir[1].filename,
					O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
		else
			fd2 = open(cmd->redir[1].filename,
					O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fd2 < 0)
			return (ctx->eno = E_OPEN, false);
		dup2(fd2, STDOUT_FILENO);
	}
	return (true);
}

static void	run_bt(t_ctx *ctx, t_cmd *cmd)
{
	t_bt_f	bt;

	bt = get_builtin(cmd);
	if (bt)
	{
		ctx->exitcode = EXIT_SUCCESS;
		if (!bt(ctx, cmd))
		{
			err_p_clear(cmd->argv[0], &ctx->eno);
			ctx->exitcode = EXIT_FAILURE;
		}
	}
}

static bool	execute(t_cmd *cmd, t_ctx *ctx)
{
	char	*valid_path;
	char	**env;

	if (get_builtin(cmd))
		return (run_bt(ctx, cmd), true);
	valid_path = get_valid_path(cmd->argv[0], ctx->env);
	if (valid_path == NULL)
		return (ctx->eno = E_CMD_NOT_FOUND, ctx->exitcode = 127, false);
	env = env_environ(ctx->env);
	if (!env)
		return (free(valid_path), ctx->eno = E_MEM, false);
	char **(argv) = cmd->argv;
	cmd->argv = NULL;
	free_cmds(ctx);
	execve(valid_path, argv, env);
	ctx->eno = E_EXECVE;
	free(valid_path);
	free(env);
	ctx->exitcode = EXIT_FAILURE;
	return (false);
}

static bool	last_cmd(int fdin, t_cmd *cmd, t_ctx *ctx)
{
	if (get_builtin(cmd))
		return (run_bt(ctx, cmd), true);
	cmd->pid = fork();
	if (cmd->pid < 0)
		return (perror("failed to fork"), false);
	if (cmd->pid == 0)
	{
		dup2(fdin, STDIN_FILENO);
		ft_close(fdin);
		if (!handle_redirection(ctx, cmd))
			err_p_clear(DEFAULT_NAME, &ctx->eno);
		if (!execute(cmd, ctx))
			err_p("last error", ctx->eno);
		env_clear(&ctx->env);
		free_cmds(ctx);
		exit(ctx->exitcode);
	}
	ft_close(fdin);
	return (true);
}

static int	child_process(int fdin, t_cmd *cmd, t_ctx *ctx)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	cmd->pid = fork();
	if (cmd->pid < 0)
		return (closepipe(pipefd), perror("failed to fork"), false);
	if (cmd->pid == 0)
	{
		dup2(fdin, STDIN_FILENO);
		dup2(pipefd[WRITE], STDOUT_FILENO);
		if (!handle_redirection(ctx, cmd))
			err_p_clear(DEFAULT_NAME, &ctx->eno);
		closepipe(pipefd);
		ft_close(fdin);
		if (!execute(cmd, ctx))
			err_p("child error", ctx->eno);
		env_clear(&ctx->env);
		free_cmds(ctx);
		exit(ctx->exitcode);
	}
	ft_close(fdin);
	ft_close(pipefd[WRITE]);
	return (pipefd[READ]);
}

int	exec_cmds(t_ctx *ctx)
{
	int		i;
	int		fdin;

	fdin = STDIN_FILENO;
	i = 0;
	while (i < ctx->cmd_count - 1)
		fdin = child_process(fdin, &ctx->cmds[i++], ctx);
	last_cmd(fdin, &ctx->cmds[i], ctx);
	i = 0;
	while (i < ctx->cmd_count)
	{
		wait(NULL);
		i++;
	}
	return (0);
}
