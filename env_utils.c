/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:04:17 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/12 14:27:57 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

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

static bool	_init_extras(t_env **env)
{
	static char	shlvl[6 + 12] = "SHLVL=";
	char		*var;
	int			n;
	static char	ps1[] = "PS1=[" DEFAULT_NAME "]$ ";

	var = env_get(*env, shlvl);
	n = 0;
	if (var)
		n = ft_atoi(var);
	if (++n <= 0 || 1000 <= n)
	{
		ft_fprintf(stderr, "%s: invalid SHLVL, resetting to 1\n", DEFAULT_NAME);
		n = 1;
	}
	ft_itoa_buf(n, shlvl + 6);
	if (!env_set(env, shlvl) || !env_set(env, ps1))
		return (false);
	return (true);
}

bool	env_init(t_env **env, char **environ)
{
	while (*environ)
	{
		if (!env_set(env, *environ))
			return (env_clear(env), false);
		environ++;
	}
	if (!_init_extras(env))
		return (env_clear(env), false);
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
