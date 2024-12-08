/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:29 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/08 12:16:00 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO check identifiers
bool	bt_unset(t_ctx *ctx, t_cmd *cmd)
{
	int	i;

	if (cmd->argc < 2)
		return (true);
	i = 1;
	while (i < cmd->argc)
		env_del(&ctx->env, cmd->argv[i++]);
	return (true);
}
