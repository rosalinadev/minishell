/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_default.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 21:33:25 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/10 22:18:43 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_default(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;

	cmdline = *parser->cmdline;
	i = 0;
	while (cmdline[i] && !is_whitespace(cmdline[i]))
		i++;
	if (!i)
		return (true);
	*token = malloc(i + 1);
	if (*token == NULL)
		return (false);
	*parser->cmdline += i;
	ft_memcpy(*token, cmdline, i);
	(*token)[i] = '\0';
	return (true);
}
