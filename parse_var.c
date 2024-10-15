/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 02:42:42 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/15 05:13:06 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO get env from proxy env
// TODO get exit code from somewhere
#define EXITCODE 42

/* Okay so, about the lines like this one:
 *     return ((bool)(*token = ft_strdup("$")));
 * This is brilliant because if malloc fails, it returns a falsy value,
 * which the calling function will correctly interpret as an error.
 * I added an unnecessary cast to clarify for the reader.
 */
bool	parse_var(t_parser *parser, char **token)
{
	char	*cmdline;
	int		i;
	bool	is_invalid_name;
	char	c;
	char	*var;

	cmdline = ++*parser->cmdline;
	i = 0;
	is_invalid_name = ft_isdigit(cmdline[i]);
	if (cmdline[i] == '?')
		return (++*parser->cmdline, (bool)(*token = ft_itoa(EXITCODE)));
	while (ft_isalnum(cmdline[i]) || cmdline[i] == '_')
		i++;
	if (i == 0 || is_invalid_name)
		return ((bool)(*token = ft_strdup("$")));
	c = cmdline[i];
	cmdline[i] = '\0';
	var = getenv(cmdline);
	cmdline[i] = c;
	*parser->cmdline += i;
	if (var == NULL)
		return (parse_token(parser, token));
	return ((bool)(*token = ft_strdup(var)));
}
