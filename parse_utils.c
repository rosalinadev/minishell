/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:02 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/13 04:01:30 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_args(t_parser *parser)
{
	int	i;

	i = -1;
	while (++i < parser->count)
		ft_printf("'%s' ", parser->tokens[i]);
	ft_printf("\n");
}

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

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

bool	sub_parser(t_parser *parent, t_token type, char **token)
{
	t_parser	parser;
	int			i;
	int			len;

	ft_printf("[sub_parser] creating context with type %d\n", type);
	get_parser(&parser, parent, type);
	if (!parse_tokens(&parser, 0))
		return (false);
	//if (parser.count == 0) // TODO figure out where to check end of string
	//	ft_printf("count is 0\n");
	ft_printf("[sub_parser] count %d: ", parser.count);
	print_args(&parser);
	i = 0;
	len = 0;
	while (i < parser.count)
		len += ft_strlen(parser.tokens[i++]);
	*token = ft_strjoin(parser.count, parser.tokens);
	ft_printf("[sub_parser] result token: '%s'\n", *token);
	return (free_parser(&parser), true);
}

