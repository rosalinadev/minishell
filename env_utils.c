/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:04:17 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/02 06:24:21 by rvandepu         ###   ########.fr       */
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

bool	env_init(t_env **env, char **environ)
{
	while (*environ)
	{
		ft_printf("[env_init] var (trunc): %.40s\n", *environ);
		if (!env_set(env, *environ))
			return (env_clear(env), false);
		environ++;
	}
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
