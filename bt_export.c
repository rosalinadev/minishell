/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:26 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/07 22:52:16 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(char *s)
{
	int	i;

	i = 0;
	if (ft_isdigit(s[i]))
		return (false);
	while (ft_isalnum(s[i]) || s[i] == '_')
		i++;
	if (s[i] == '=' || s[i] == '\0')
		return (true);
	return (false);
}

static bool	handle_argument(t_ctx *ctx, char **arg)
{
	char	*str;

	if (!is_valid_identifier(*arg))
		return (eno(ctx, E_ARGS_IDENTIFIER), false);
	if (!ft_strchr(*arg, '='))
	{
		str = env_get(ctx->env, *arg);
		str = ft_strjoinv(3 - (str == NULL), *arg, "=", str);
		if (str == NULL)
			return (eno(ctx, E_MEM), false);
		free(*arg);
		*arg = str;
	}
	if (!env_set(&ctx->env, *arg))
		return (eno(ctx, E_MEM), false);
	return (true);
}

// TODO :)
bool	bt_export(t_ctx *ctx, t_cmd *cmd)
{
	int		argi;

	if (cmd->argc < 2)
		return (bt_env(ctx, cmd));
	argi = 1;
	while (argi < cmd->argc)
	{
		if (!handle_argument(ctx, &cmd->argv[argi]))
		{
			ft_fprintf(stderr, "export: '%s': ", cmd->argv[argi]);
			err_p_clear(NULL, &ctx->err);
		}
		argi++;
	}
	return (true);
}
