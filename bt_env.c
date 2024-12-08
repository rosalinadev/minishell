/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:31 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/08 16:03:07 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	bt_env(t_ctx *ctx, t_cmd *cmd)
{
	t_env	*var;

	(void)cmd;
	var = ctx->env;
	while (var)
	{
		if (_env_namecmp(var->var, "PS1") != 0
			&& _env_namecmp(var->var, "PS2") != 0)
			ft_printf("%s\n", var->var);
		var = var->next;
	}
	return (true);
}
