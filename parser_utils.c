/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 06:49:12 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/21 17:53:12 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd *cmd, bool free_struct)
{
	int	i;

	i = 0;
	while (i < cmd->argc)
		free(cmd->argv[i++]);
	free(cmd->argv);
	free(cmd->redir[0].filename);
	free(cmd->redir[1].filename);
	if (free_struct)
		free(cmd);
}

char	*dup_token(char *src, int len)
{
	char	*token;

	token = malloc(len + 1);
	if (token == NULL)
		return (token);
	ft_memcpy(token, src, len);
	token[len] = '\0';
	return (token);
}

void	get_parser(t_parser *parser, t_parser *parent, t_token type)
{
	*parser = (t_parser){};
	if (parent)
		*parser = *parent;
	parser->type = type;
	parser->count = 0;
	parser->tokens = NULL;
}

void	free_parser(t_parser *parser)
{
	while (parser->count--)
		free(parser->tokens[parser->count]);
	free(parser->tokens);
}

// TODO delete
static void	print_args(t_parser *parser)
{
	int	i;

	i = -1;
	while (++i < parser->count)
		ft_printf("'%s' ", parser->tokens[i]);
	ft_printf("\n");
}

bool	sub_parser(t_parser *parent, t_token type, char **token)
{
	t_parser	parser;

	ft_printf("[sub_parser] creating context with type %d\n", type);
	get_parser(&parser, parent, type);
	if (!parse_tokens(&parser, 0))
		return (false);
	ft_printf("[sub_parser] count %d: ", parser.count);
	print_args(&parser);
	if (parser.type != T_DOUBLEQUOTE && parser.count == 0)
		return (free_parser(&parser), true);
	*token = ft_strjoin(parser.count, parser.tokens);
	ft_printf("[sub_parser] result token: '%s'\n", *token);
	return (free_parser(&parser), *token != NULL);
}
