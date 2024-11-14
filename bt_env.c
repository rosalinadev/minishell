/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:31 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/14 01:50:59 by rvandepu         ###   ########.fr       */
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
		ft_printf("%s\n", var->var);
		var = var->next;
	}
	return (true);
}
