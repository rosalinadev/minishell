/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:46:30 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/01 07:51:06 by rvandepu         ###   ########.fr       */
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
	[E_CHDIR] = "Could not change directory", \
	[E_CMD_NOT_FOUND] = "Command not found", \
	[E_HOMEUNSET] = "HOME not set", \
};

static const bool	g_err_perror[E__MAX] = {\
	[E_MEM] = true, \
	[E_OPEN] = true, \
	[E_EXECVE] = true, \
	[E_PIPE] = true, \
	[E_FORK] = true, \
	[E_CHDIR] = true, \
};

void	err_p(const char *s, t_err *err)
{
	bool		valid_eno;
	const char	*errstr;
	char		*eol;

	if (err->eno == E__NOPRINT)
		return ;
	valid_eno = E__UNKNOWN < err->eno && err->eno < E__MAX;
	errstr = g_errstr[E__UNKNOWN];
	if (valid_eno && g_errstr[err->eno])
		errstr = g_errstr[err->eno];
	eol = "\n";
	if (valid_eno && g_err_perror[err->eno])
		eol = ": ";
	if (s && *s)
		ft_fprintf(stderr, "%s: %s%s", s, errstr, eol);
	else
		ft_fprintf(stderr, "%s%s", errstr, eol);
	if (valid_eno && g_err_perror[err->eno])
	{
		errno = err->errsv;
		perror(NULL);
	}
}

void	err_p_clear(const char *s, t_err *err)
{
	err_p(s, err);
	err->eno = E__UNKNOWN;
}

void	eno(t_ctx *ctx, t_eno eno)
{
	ctx->err.eno = E__UNKNOWN;
	if (!(E__UNKNOWN < eno && eno < E__MAX))
		return ;
	ctx->err.eno = eno;
	if (g_err_perror[eno])
		ctx->err.errsv = errno;
	if (eno == E_MEM)
		ctx->should_exit = true;
}
