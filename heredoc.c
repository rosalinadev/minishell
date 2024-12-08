/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 11:35:00 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/08 12:28:06 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_last_sig;

static bool	get_line(t_ctx *ctx, char **line)
{
	g_last_sig = 0;
	set_signals(S_HEREDOC);
	ft_fprintf(rl_outstream, "> ");
	*line = get_next_line(STDIN_FILENO);
	set_signals(S_IGNORE);
	if (g_last_sig == SIGINT || *line == NULL || ft_strchr(*line, '\n') == NULL)
	{
		eno(ctx, E_HD_NO_DELIM);
		if (g_last_sig == SIGINT)
			eno(ctx, E__NOPRINT);
		return (ft_fprintf(rl_outstream, "\n"), free(*line), false);
	}
	return (true);
}

bool	parse_heredoc(t_parser *parser, char **token)
{
	char	*line;
	int		i;

	line = *parser->cmdline;
	i = 0;
	if (line[i] == '\0')
		return (true);
	else if (line[i] == '$')
		return (parse_var(parser, token));
	while (line[i] && line[i] != '$')
		i++;
	*token = dup_token(line, i);
	if (*token == NULL)
		return (eno(parser->ctx, E_MEM), false);
	*parser->cmdline += i;
	return (true);
}

static bool	expand_line(t_ctx *ctx, char **line)
{
	char		*s;
	t_parser	parser;

	get_parser(&parser, NULL, P_HEREDOC);
	parser.ctx = ctx;
	s = *line;
	parser.cmdline = &s;
	if (!parse_tokens(&parser, 0))
		return (false);
	free(*line);
	*line = ft_strjoin(parser.count, parser.tokens);
	free_parser(&parser);
	if (*line == NULL)
		return (eno(ctx, E_MEM), false);
	return (true);
}

static bool	read_heredoc(t_ctx *ctx, t_redir *redir, int fd)
{
	char	*line;
	char	*lf;

	while (true)
	{
		if (!get_line(ctx, &line))
			return (false);
		lf = ft_strchr(line, '\n');
		*lf = '\0';
		if (ft_strcmp(line, redir->filename) == 0)
			return (free(line), true);
		*lf = '\n';
		if (!redir->quoted && !expand_line(ctx, &line))
			return (free(line), false);
		if (write(fd, line, ft_strlen(line)) == -1)
			return (eno(ctx, E_WRITE), free(line), false);
		free(line);
	}
}

#define HEREDOC_PATH "/tmp/mshd-"

bool	read_heredocs(t_ctx *ctx)
{
	static char	path[sizeof(HEREDOC_PATH) + 12] = HEREDOC_PATH;
	static int	path_n;
	int			i;
	int			fd;

	i = -1;
	while (++i < ctx->cmd_count)
	{
		if (!ctx->cmds[i].redir[0].read_heredoc)
			continue ;
		ft_itoa_buf(path_n, path + sizeof(HEREDOC_PATH) - 1);
		if (++path_n < 0)
			path_n = 0;
		fd = open(path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		if (fd == -1)
			return (eno(ctx, E_OPEN), false);
		if (!read_heredoc(ctx, ctx->cmds[i].redir, fd))
			return (free_gnl(), close(fd), unlink(path), false);
		(close(fd), free(ctx->cmds[i].redir[0].filename));
		ctx->cmds[i].redir[0].filename = ft_strdup(path);
		if (ctx->cmds[i].redir[0].filename == NULL)
			return (eno(ctx, E_MEM), unlink(path), false);
		ctx->cmds[i].redir[0].is_heredoc = true;
	}
	return (free_gnl(), true);
}
