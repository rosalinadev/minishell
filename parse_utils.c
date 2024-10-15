/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:14:02 by rvandepu          #+#    #+#             */
/*   Updated: 2024/10/15 09:34:32 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dup_token(char *src, int len)
{
	char	*token;

	token = malloc(len + 1);
	if (token == NULL)
		return (token);
	ft_memcpy(token, src, len);
	token[len] = '\0';
	return (token);
}
