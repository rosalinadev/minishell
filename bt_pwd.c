/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:18 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/14 02:10:28 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	bt_pwd(t_ctx *ctx, t_cmd *cmd)
{
	char	cwd[2048];

	if (getcwd(cwd, sizeof(cwd)) != 0)
		printf("%s\n", cwd);
	else
		printf("getcwd error\n");
	return (false);
}
