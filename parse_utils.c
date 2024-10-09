/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:02 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/08 22:49:22 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_parser(t_parser *parser, t_parser *parent, t_token type)
{
	*parser = (t_parser){};
	if (parent)
	{
		parser->redir = parent->redir;
		parser->cmdline = parent->cmdline;
	}
	parser->parent = parent;
	parser->type = type;
}

void	free_parser(t_parser *parser)
{
	while (parser->count--)
		free(parser->tokens[parser->count]);
	free(parser->tokens);
}

void	skip_whitespace(char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
}

bool	sub_parser(t_parser *parent, t_token type, char **token)
{
	t_parser	parser;
	int			i;
	int			len;

	get_parser(&parser, parent, type);
	if (!parse_tokens(&parser, 0))
		return (false);
	if (parser.count == 0) // TODO figure out where to check end of string
		ft_printf("count is 0\n");
	i = 0;
	len = 0;
	while (i < parser.count)
		len += ft_strlen(parser.tokens[i++]);
	*token = malloc(len + 1);
	if (*token == NULL)
		return (free_parser(&parser), false);
	i = 0;
	while (i < parser.count)
		ft_strlcat(*token, parser.tokens[i++], len);
	return (free_parser(&parser), true);
}

