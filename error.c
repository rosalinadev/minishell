/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:46:30 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/14 02:16:15 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*err_str(t_err eno)
{
	static const char	*errstr[E__MAX] = {\
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
		[E_EXECVE] = "execve failed", \
		[E_CMD_NOT_FOUND] = "Command not found", \
	};

	if (E__UNKNOWN < eno && eno < E__MAX && errstr[eno])
		return (errstr[eno]);
	return (errstr[E__UNKNOWN]);
}

void	err_p(const char *s, t_err eno)
{
	const char	*errstr;

	errstr = err_str(eno);
	if (s && *s)
		ft_fprintf(stderr, "%s: %s\n", s, errstr);
	else
		ft_fprintf(stderr, "%s\n", errstr);
}

void	err_p_clear(const char *s, t_err *eno)
{
	err_p(s, *eno);
	*eno = E__UNKNOWN;
}
