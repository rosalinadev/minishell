/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_default.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 21:33:25 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/13 11:26:18 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//#define DELIM " \t\'\"$"
#define DELIM " \t\'"

static bool	parse_quote(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	cmdline = ++(*parser->cmdline);
	i = 0;
	while (cmdline[i] && cmdline[i] != '\'')
		i++;
	if (!cmdline[i])
		return (false);
	*token = dup_token(cmdline, i);
	if (*token == NULL)
		return (false);
	ft_printf("[parse_quote] token: \"%s\" len:%d\n", *token, i);
	*parser->cmdline += i + 1;
	return (true);
}

static bool	parse_special(t_parser *parser, char **token)
{
	ft_printf("[parse_special] found %c\n", **parser->cmdline);
	if (**parser->cmdline == '\'')
		return (parse_quote(parser, token));
	else
		(*parser->cmdline)++;
	return (true);
}

bool	parse_default(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	cmdline = *parser->cmdline;
	i = 0;
	if (ft_in(cmdline[i], DELIM) && !is_whitespace(cmdline[i]))
		return (parse_special(parser, token));
	while (cmdline[i] && !ft_in(cmdline[i], DELIM))
		i++;
	if (!i)
		return (true);
	*token = dup_token(cmdline, i);
	if (*token == NULL)
		return (false);
	*parser->cmdline += i;
	return (true);
}
