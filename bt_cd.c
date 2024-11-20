/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:14 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/19 12:44:00 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO PWD var
// TODO OLDPWD var
// TODO - reads OLDPWD
bool	bt_cd(t_ctx *ctx, t_cmd *cmd)
{
	char	*dir;

	if (cmd->argc > 2)
		return (ctx->eno = E_ARGS_TOO_MANY, false);
	dir = cmd->argv[1];
	//if (dir && ft_strcmp(dir, "-"))
	if (dir == NULL)
		dir = env_get(ctx->env, "HOME");
	if (dir == NULL)
		return (ctx->eno = E_HOMEUNSET, false);
	if (chdir(dir) < 0)
		return (perror("cd"), ctx->eno = E__NOPRINT, false);
	return (true);
}
