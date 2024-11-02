/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 22:25:02 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/02 02:02:26 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdbool.h>
# include <stdlib.h>
# include "libft.h"

extern char	**environ;

typedef struct s_env
{
	struct s_env	*next;
	char			*var;
	char			*val;
}	t_env;

// env.c
bool	env_init(t_env **env, char **environ);
bool	env_set(t_env **env, char *var, bool del);
char	*env_get(t_env *env, char *name);
void	env_clear(t_env **env);
char	**env_environ(t_env *start);

// env_utils.c
int		_env_namecmp(char *var1, char *var2);

#endif
