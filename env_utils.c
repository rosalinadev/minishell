/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:04:17 by rvandepu          #+#    #+#             */
/*   Updated: 2024/11/02 01:10:53 by rvandepu         ###   ########.fr       */
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
