/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:19:35 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/07 20:54:17 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define REDIRECT "><"

static bool	parse_redirect(t_parser *parser, t_redir *redir)
{
	while (**parser->cmdline && ft_in(**parser->cmdline, WHITESPACE))
		++*parser->cmdline;
	parser->parsing_redir = redir;
	if (!sub_parser(parser, P_ARGUMENT, &redir->filename)
		|| redir->filename == NULL)
	{
		if (redir->read_heredoc)
			eno(parser->ctx, E_HD_EXP_DELIM);
		else
			eno(parser->ctx, E_RD_EXP_FILENAME);
		return (parser->ctx->exitcode = EXIT_PARSER_FAILURE, false);
	}
	parser->parsing_redir = NULL;
	parser->has_skipped = true;
	return (true);
}

static bool	parse_redirects(t_parser *parser)
{
	if (**parser->cmdline == '<')
	{
		parser->redir[0] = (free(parser->redir[0].filename), (t_redir){});
		if ((*parser->cmdline)[1] == '<')
			return (*parser->cmdline += 2, parser->redir[0].read_heredoc = true,
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

bool	parse_command(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	cmdline = *parser->cmdline;
	i = 0;
	while (ft_in(cmdline[i], WHITESPACE))
		i++;
	*parser->cmdline += i;
	if (!cmdline[i] || cmdline[i] == '|')
		return (true);
	if (ft_in(cmdline[i], REDIRECT))
		return (parse_redirects(parser));
	return (sub_parser(parser, P_ARGUMENT, token));
}
