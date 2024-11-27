/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 08:30:57 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/27 04:14:27 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const t_bt_f	*_bt(void)
{
	static const t_bt_f		bt[BT__MAX] = {\
		[BT_DBG_CMDS] = bt_dbg_cmds, \
		[BT_ECHO] = bt_echo, \
		[BT_CD] = bt_cd, \
		[BT_PWD] = bt_pwd, \
		[BT_EXPORT] = bt_export, \
		[BT_UNSET] = bt_unset, \
		[BT_ENV] = bt_env, \
		[BT_EXIT] = bt_exit, \
	};

	return (bt);
}

static const char *const	*_bt_str(void)
{
	static const char *const	bt_str[BT__MAX] = {\
		[BT_DBG_CMDS] = "_debug:cmds", \
		[BT_ECHO] = "echo", \
		[BT_CD] = "cd", \
		[BT_PWD] = "pwd", \
		[BT_EXPORT] = "export", \
		[BT_UNSET] = "unset", \
		[BT_ENV] = "env", \
		[BT_EXIT] = "exit", \
	};

	return (bt_str);
}

t_bt_f	get_builtin(t_cmd *cmd)
{
	t_bt	bt;

	if (!cmd->argc)
		return (NULL);
	bt = BT__FIRST;
	while (bt < BT__MAX)
		if (ft_strcmp(cmd->argv[0], _bt_str()[bt++]) == 0)
			return (_bt()[--bt]);
	return (NULL);
}
