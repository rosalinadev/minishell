/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:04:17 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/07 22:51:42 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	_env_namecmp(char *var1, char *var2)
{
	char	*var1_cut;
	char	*var2_cut;
	int		ret;

	var1_cut = ft_strchr(var1, '=');
	var2_cut = ft_strchr(var2, '=');
	if (var1_cut)
		*var1_cut = '\0';
	if (var2_cut)
		*var2_cut = '\0';
	ret = ft_strcmp(var1, var2);
	if (var1_cut)
		*var1_cut = '=';
	if (var2_cut)
		*var2_cut = '=';
	return (ret);
}

static bool	_init_extras(t_ctx *ctx)
{
	static char	shlvl[6 + 12] = "SHLVL=";
	char		*var;
	int			n;
	static char	ps1[] = "PS1=["SHELL_NAME"]$ ";

	var = env_get(ctx->env, shlvl);
	n = 0;
	if (var)
		n = ft_atoi(var);
	if (++n <= 0 || 1000 <= n)
	{
		ft_fprintf(stderr, "%s: invalid SHLVL, resetting to 1\n", SHELL_NAME);
		n = 1;
	}
	ft_itoa_buf(n, shlvl + 6);
	if (!env_set(&ctx->env, shlvl) || !env_set(&ctx->env, ps1))
		return (eno(ctx, E_MEM), false);
	return (true);
}

bool	env_init(t_ctx *ctx, char **environ)
{
	while (*environ)
	{
		if (!env_set(&ctx->env, *environ))
			return (eno(ctx, E_MEM), env_clear(&ctx->env), false);
		environ++;
	}
	if (!_init_extras(ctx))
		return (env_clear(&ctx->env), false);
	return (true);
}

void	env_clear(t_env **env)
{
	t_env	*next;

	while (*env)
	{
		next = (*env)->next;
		free((*env)->var);
		free(*env);
		*env = next;
	}
}
