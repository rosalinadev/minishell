/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:14 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/14 02:10:19 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	bt_cd(t_ctx *ctx, t_cmd *cmd)
{
	char	*home;

	home = env_get(ctx->env, "HOME");
	bt_pwd(ctx, cmd);
	chdir(home);
	bt_pwd(ctx, cmd);
	if (getenv("HOME") == 0)
	{
		perror("chdir() error");
	}
	return (false);
}
