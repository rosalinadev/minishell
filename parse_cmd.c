/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:19:35 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/13 08:49:12 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DELIM "|<>"

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
	while (is_whitespace(cmdline[i]))
		i++;
	if (!cmdline[i])
		return (true);
	//if (ft_in(cmdline[i], DELIM))
	//	return (parse_redirect());
	*parser->cmdline += i;
	return (sub_parser(parser, T_DEFAULT, token));
}
