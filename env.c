/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:04:18 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/02 07:48:17 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	env_set(t_ctx *ctx, char *var)
{
	t_env	**env;

	env = &ctx->env;
	while (*env && _env_namecmp(var, (*env)->var) != 0)
		env = &(*env)->next;
	var = ft_strdup(var);
	if (var == NULL)
		return (eno(ctx, E_MEM), false);
	if (*env == NULL)
		*env = ft_calloc(1, sizeof(t_env));
	if (*env == NULL)
		return (eno(ctx, E_MEM), free(var), false);
	free((*env)->var);
	(*env)->var = var;
	(*env)->val = ft_strchr((*env)->var, '=');
	if ((*env)->val++ == NULL)
		(*env)->val = "";
	return (true);
}

bool	env_del(t_env **env, char *name)
{
	t_env	*next;

	while (*env && _env_namecmp(name, (*env)->var) != 0)
		env = &(*env)->next;
	if (*env == NULL)
		return (false);
	next = (*env)->next;
	free((*env)->var);
	free(*env);
	*env = next;
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
