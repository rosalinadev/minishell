/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:46:30 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/07 19:27:33 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*g_errstr[E__MAX] = {\
	[E__UNKNOWN] = "Unknown error", \
	[E_MEM] = "Memory allocation failed", \
	[E_OPEN] = "Could not open file", \
	[E_EXP_CMD_PIPE] = "Expected a command but found a pipe", \
	[E_PIPE_EXP_CMD] = "Expected a command after pipe", \
	[E_HD_EXP_DELIM] = "Expected a delimiter after heredoc", \
	[E_RD_EXP_FILENAME] = "Expected a filename after redirection", \
	[E_QUOTES] = "Unmatched quotes", \
	[E_ARGS_IDENTIFIER] = "Invalid identifier", \
	[E_ARGS_TOO_FEW] = "Too few arguments", \
	[E_ARGS_TOO_MANY] = "Too many arguments", \
	[E_ARGS_NUMERIC] = "Argument must be numeric", \
	[E_EXECVE] = "execve() failed", \
	[E_PIPE] = "pipe() failed", \
	[E_FORK] = "fork() failed", \
	[E_WRITE] = "write() failed", \
	[E_CHDIR] = "Could not change directory", \
	[E_CMD_NOT_FOUND] = "Command not found", \
	[E_CMD_IS_DIR] = "Is a directory", \
	[E_HOMEUNSET] = "HOME not set", \
	[E_HD_NO_DELIM] = "No delimiter ending here-document", \
};

static const bool	g_err_perror[E__MAX] = {\
	[E_MEM] = true, \
	[E_OPEN] = true, \
	[E_EXECVE] = true, \
	[E_PIPE] = true, \
	[E_FORK] = true, \
	[E_WRITE] = true, \
	[E_CHDIR] = true, \
};

void	err_p(const char *s, t_err *err)
{
	bool		valid_eno;
	const char	*errstr;

	if (err->eno == E__NOPRINT)
		return ;
	valid_eno = E__UNKNOWN < err->eno && err->eno < E__MAX;
	errstr = g_errstr[E__UNKNOWN];
	if (valid_eno && g_errstr[err->eno])
		errstr = g_errstr[err->eno];
	if (valid_eno && g_err_perror[err->eno])
	{
		if (s && *s)
			ft_fprintf(stderr, "%s: %s: %s\n", s, errstr, strerror(err->errsv));
		else
			ft_fprintf(stderr, "%s: %s\n", errstr, strerror(err->errsv));
		return ;
	}
	if (s && *s)
		ft_fprintf(stderr, "%s: %s\n", s, errstr);
	else
		ft_fprintf(stderr, "%s\n", errstr);
}

void	err_p_clear(const char *s, t_err *err)
{
	err_p(s, err);
	err->eno = E__UNKNOWN;
}

void	eno(t_ctx *ctx, t_eno eno)
{
	ctx->err.eno = eno;
	ctx->err.errsv = errno;
	if (eno == E_MEM)
		ctx->should_exit = true;
}

void	enosv(t_ctx *ctx, t_eno errnum, int errsv)
{
	eno(ctx, errnum);
	ctx->err.errsv = errsv;
}
