/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:33:26 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/08 16:38:29 by rvandepu         ###   ########.fr       */
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
	if (i && (s[i] == '=' || s[i] == '\0'))
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

#define BLUE "\x1b[48;2;91;206;250m"
#define PINK "\x1b[48;2;245;169;184m"
#define WHITE "\x1b[48;2;255;255;255m"
#define BLACKFG "\x1b[30m"
#define BOLD "\x1b[1m"
#define RESET "\x1b[0m"

static bool	propaganda(t_ctx *ctx)
{
	static char	ps1[] = "PS1="BLACKFG BOLD
		BLUE"T"PINK"R"WHITE"A"PINK"N"BLUE"S"RESET"$ ";
	static char	ps2[] = "PS2=🐱> ";
	bool		ret;

	ret = true;
	if (!env_set(&ctx->env, ps1) || !env_set(&ctx->env, ps2))
		ret = (eno(ctx, E_MEM), false);
	ft_printf(BLUE"%18s"RESET"\n", "");
	ft_printf(PINK"%18s"RESET"\n", "");
	ft_printf(WHITE BLACKFG BOLD"%9s%-9s"RESET"\n", "Trans ", "rights");
	ft_printf(PINK"%18s"RESET"\n", "");
	ft_printf(BLUE"%18s"RESET"\n", "");
	return (ret);
}

bool	bt_export(t_ctx *ctx, t_cmd *cmd)
{
	int		argi;
	bool	ret;

	if (cmd->argc < 2)
		return (propaganda(ctx));
	ret = true;
	argi = 1;
	while (argi < cmd->argc)
	{
		if (!handle_argument(ctx, &cmd->argv[argi]))
		{
			ft_fprintf(stderr, "export: '%s': ", cmd->argv[argi]);
			err_p_clear(NULL, &ctx->err);
			ret = false;
		}
		argi++;
	}
	return (ret);
}
