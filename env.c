/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:04:18 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/02 02:06:50 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

// TODO move to main and merge utils here?
bool	env_init(t_env **env, char **environ)
{
	while (*environ)
	{
		ft_printf("[env_init] var (trunc): %.40s\n", *environ);
		if (!env_set(env, *environ, false))
			return (env_clear(env), false);
		environ++;
	}
	return (true);
}

bool	env_set(t_env **env, char *var, bool del)
{
	t_env	*next;

	while (*env && _env_namecmp(var, (*env)->var) != 0)
		env = &(*env)->next;
	if (del)
	{
		ft_printf("[env_set] deleting var (trunc): %.40s\n", var);
		if (*env)
			free((*env)->var);
		next = (*env)->next;
		free(*env);
		*env = next;
		return (true);
	}
	var = ft_strdup(var);
	if (var == NULL)
		return (false);
	ft_printf("[env_set] *env: %p\n", *env);
	if (*env == NULL)
		*env = ft_calloc(1, sizeof(t_env));
	if (*env == NULL)
		return (free(var), false);
	ft_printf("[env_set] old var (trunc): %.40s\n", (*env)->var);
	free((*env)->var);
	(*env)->var = var;
	ft_printf("[env_set] new var (trunc): %.40s\n", (*env)->var);
	(*env)->val = ft_strchr((*env)->var, '=');
	if ((*env)->val++ == NULL)
	{
		ft_printf("[env_set] no val, setting to empty\n");
		(*env)->val = "";
	}
	return (true);
}

char	*env_get(t_env *env, char *name)
{
	while (env && _env_namecmp(name, env->var) != 0)
		env = env->next;
	if (env)
		return (env->val);
	return (NULL);
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

char	**env_environ(t_env *start)
{
	t_env	*env;
	int		i;
	char	**ret;

	env = start;
	i = 0;
	while (env)
		env = (i++, env->next);
	ret = ft_calloc(i + 1, sizeof(char *));
	if (ret == NULL)
		return (NULL);
	env = start;
	i = 0;
	while (env)
	{
		ret[i++] = env->var;
		env = env->next;
	}
	return (ret);
}
