/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:19:35 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/19 15:59:40 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define REDIRECT "><"

static bool	parse_redirect(t_parser *parser, t_redir *redir)
{
	while (**parser->cmdline && ft_in(**parser->cmdline, WHITESPACE))
		++*parser->cmdline;
	parser->parsing_redir = redir;
	if (!sub_parser(parser, T_DEFAULT, &redir->filename)
		|| redir->filename == NULL)
		return (false);
	parser->parsing_redir = NULL;
	return (true);
}

static bool	parse_redirects(t_parser *parser)
{
	if (**parser->cmdline == '<')
	{
		parser->redir[0] = (free(parser->redir[0].filename), (t_redir){});
		if ((*parser->cmdline)[1] == '<')
			return (*parser->cmdline += 2, parser->redir[0].is_heredoc = true,
				parse_redirect(parser, &parser->redir[0]));
		return (++*parser->cmdline, parse_redirect(parser, &parser->redir[0]));
	}
	else if (**parser->cmdline == '>')
	{
		parser->redir[1] = (free(parser->redir[1].filename), (t_redir){});
		if ((*parser->cmdline)[1] == '>')
			return (*parser->cmdline += 2, parser->redir[1].append = true,
				parse_redirect(parser, &parser->redir[1]));
		return (++*parser->cmdline, parse_redirect(parser, &parser->redir[1]));
	}
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
	*parser->cmdline += i;
	if (ft_in(cmdline[i], REDIRECT))
		return (parse_redirects(parser));
	return (sub_parser(parser, T_DEFAULT, token));
}
