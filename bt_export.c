/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:26 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/14 02:07:42 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_argument(t_ctx *ctx, char **arg)
{
	char	*str;

	if (!ft_strchr(*arg, '='))
	{
		str = env_get(ctx->env, *arg);
		str = ft_strjoinv(3 - (str == NULL), *arg, "=", str);
		if (str == NULL)
			return (ctx->eno = E_MEM, false);
		free(*arg);
		*arg = str;
	}
	if (!env_set(&ctx->env, *arg))
		return (ctx->eno = E_MEM, false);
	return (true);
}

// TODO :)
// TODO identifier check
bool	bt_export(t_ctx *ctx, t_cmd *cmd)
{
	int		argi;

	if (cmd->argc < 2)
		return (bt_env(ctx, cmd));
	argi = 1;
	while (argi < cmd->argc)
	{
		if (!handle_argument(ctx, &cmd->argv[argi]))
			err_p_clear("export", &ctx->eno);
		argi++;
	}
	return (true);
}
