/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:33 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/14 01:49:56 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	bt_exit(t_ctx *ctx, t_cmd *cmd)
{
	int	i;

	if (cmd->argc < 2)
		return (ctx->should_exit = true, true);
	if (cmd->argc > 2)
		return (ctx->eno = E_ARGS_TOO_MANY, false);
	i = 0;
	if (ft_in(cmd->argv[1][i], "+-"))
		i++;
	while (ft_isdigit(cmd->argv[1][i]))
		i++;
	if (cmd->argv[1][i])
		return (ctx->eno = E_ARGS_NUMERIC, false);
	ctx->should_exit = true;
	ctx->exitcode = ft_atoi(cmd->argv[1]);
	return (true);
}
