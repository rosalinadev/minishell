/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:18 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/01 07:51:20 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO test deleted cwd
bool	bt_pwd(t_ctx *ctx, t_cmd *cmd)
{
	char	*cwd;

	(void)cmd;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (perror("pwd"), eno(ctx, E__NOPRINT), false); // TODO get rid, see above
	ft_printf("%s\n", cwd);
	free(cwd);
	return (true);
}
