/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:29 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/14 01:54:37 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	bt_unset(t_ctx *ctx, t_cmd *cmd)
{
	int	i;

	if (cmd->argc < 2)
		return (ctx->eno = E_ARGS_TOO_FEW, false);
	i = 1;
	while (i < cmd->argc)
		env_del(&ctx->env, cmd->argv[i++]);
	return (true);
}
