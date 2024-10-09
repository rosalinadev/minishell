/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:19:35 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/08 22:49:19 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static bool	parse_redirect(t_parser *parser, char *cmdline)
{
	int i;
}*/

bool	parse_cmd(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	cmdline = *parser->cmdline;
	i = 0;
	skip_whitespace(cmdline, &i);
	//if (cmdline[i] == '<' || cmdline[i] == '>')
	//	parse_redirect();
	*parser->cmdline += i;
	sub_parser(parser, T_DEFAULT, token);
}
