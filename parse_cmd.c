/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:19:35 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/18 00:00:51 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define REDIRECT "><"

static bool	parse_redirect(t_parser *parser, bool is_output)
{
	char	*cmdline;
	int		i;
	char	**token;

	if (is_output)
		token = &parser->out->filename;
	else
		token = &parser->in->filename;
	cmdline = *parser->cmdline;
	i = 0;
	while (cmdline[i] && ft_in(cmdline[i], WHITESPACE))
		i++;
	return (false);
}

static bool	parse_redirects(t_parser *parser)
{
	if (**parser->cmdline == '<')
		return (parse_redirect(parser, false));
	else if (ft_strncmp(*parser->cmdline, "<<", 2) == 0)
		return (parse_redirect(parser, false));
	else if (ft_strncmp(*parser->cmdline, "<<-", 3) == 0)
		return (parse_redirect(parser, false));
	else if (**parser->cmdline == '>')
		return (parse_redirect(parser, true));
	else if (ft_strncmp(*parser->cmdline, ">>", 2) == 0)
		return (parse_redirect(parser, true));
	return (false);
}

bool	parse_cmd(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	cmdline = *parser->cmdline;
	i = 0;
	while (ft_in(cmdline[i], WHITESPACE))
		i++;
	if (!cmdline[i] || cmdline[i] == '|')
		return (true);
	else if (false && ft_in(cmdline[i], REDIRECT))
		return (parse_redirects(parser));
	*parser->cmdline += i;
	return (sub_parser(parser, T_DEFAULT, token));
}
