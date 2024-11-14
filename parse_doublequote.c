/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_doublequote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:07:53 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/11 16:26:09 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DELIM "\"$"

bool	parse_doublequote(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	if (parser->parsing_redir)
		parser->parsing_redir->quoted = true;
	cmdline = *parser->cmdline;
	i = 0;
	if (cmdline[i] == '$')
		return (parse_var(parser, token));
	else if (cmdline[i] == '"')
		return (++*parser->cmdline, true);
	while (cmdline[i] && !ft_in(cmdline[i], DELIM))
		i++;
	if (!cmdline[i])
		return (parser->ctx->eno = E_QUOTES, false);
	*token = dup_token(cmdline, i);
	if (*token == NULL)
		return (parser->ctx->eno = E_MEM, false);
	return (*parser->cmdline += i, true);
}
