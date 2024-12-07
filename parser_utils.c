/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 06:49:12 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/07 20:57:25 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (cmd->argv)
		while (cmd->argc)
			free(cmd->argv[--cmd->argc]);
	free(cmd->argv);
	if (cmd->redir[0].is_heredoc)
		unlink(cmd->redir[0].filename);
	free(cmd->redir[0].filename);
	free(cmd->redir[1].filename);
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

void	get_parser(t_parser *parser, t_parser *parent, t_parse type)
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

bool	sub_parser(t_parser *parent, t_parse type, char **token)
{
	t_parser	parser;

	get_parser(&parser, parent, type);
	if (!parse_tokens(&parser, 0))
		return (false);
	if (parser.type != P_DOUBLEQUOTE && parser.count == 0)
		return (free_parser(&parser), parent->has_skipped = true, true);
	*token = ft_strjoin(parser.count, parser.tokens);
	if (*token == NULL)
		eno(parent->ctx, E_MEM);
	return (free_parser(&parser), *token != NULL);
}
