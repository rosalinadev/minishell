/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:29 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/08 16:59:56 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(char *s)
{
	int	i;

	i = 0;
	if (ft_isdigit(s[i]))
		return (false);
	while (ft_isalnum(s[i]) || s[i] == '_')
		i++;
	if (i == 0 || s[i] != '\0')
		return (false);
	return (true);
}

bool	bt_unset(t_ctx *ctx, t_cmd *cmd)
{
	int		i;

	i = 1;
	while (i < cmd->argc)
	{
		if (is_valid_identifier(cmd->argv[i]))
			env_del(&ctx->env, cmd->argv[i]);
		i++;
	}
	return (true);
}
