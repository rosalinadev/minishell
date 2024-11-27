/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_default.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 21:33:25 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/25 04:34:40 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	parse_quote(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	if (parser->parsing_redir)
		parser->parsing_redir->quoted = true;
	cmdline = ++*parser->cmdline;
	i = 0;
	while (cmdline[i] && cmdline[i] != '\'')
		i++;
	if (!cmdline[i])
		return (parser->ctx->exitcode = EXIT_PARSER_FAILURE,
			eno(parser->ctx, E_QUOTES), false);
	*token = dup_token(cmdline, i);
	if (*token == NULL)
		return (eno(parser->ctx, E_MEM), false);
	*parser->cmdline += i + 1;
	return (true);
}

static bool	parse_special(t_parser *parser, char **token)
{
	if (**parser->cmdline == '\'')
		return (parse_quote(parser, token));
	else if (**parser->cmdline == '$')
		return (parse_var(parser, token));
	else if (**parser->cmdline == '"')
		return (++*parser->cmdline, sub_parser(parser, T_DOUBLEQUOTE, token));
	else if (**parser->cmdline == '<' || **parser->cmdline == '>')
		return (true);
	else if (**parser->cmdline == '|')
		return (true);
	return (false);
}

#define DELIM "\'\"$<>|"

bool	parse_default(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	cmdline = *parser->cmdline;
	i = 0;
	if (ft_in(cmdline[i], DELIM))
		return (parse_special(parser, token));
	while (cmdline[i] && !ft_in(cmdline[i], WHITESPACE DELIM))
		i++;
	if (!i)
		return (true);
	*token = dup_token(cmdline, i);
	if (*token == NULL)
		return (eno(parser->ctx, E_MEM), false);
	*parser->cmdline += i;
	return (true);
}
