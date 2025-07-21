/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:26:31 by marvin            #+#    #+#             */
/*   Updated: 2025/07/21 08:26:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/expend.h"

int	append_char(char **dst, char c)
{
	char	*new;
	size_t	len;

	len = 0;
	if (*dst)
		len = ft_strlen(*dst);
	new = malloc(len + 2);
	if (!new)
	{
		if (*dst)
		{
			free(*dst);
			*dst = NULL;
		}
		return (-1);
	}
	if (*dst)
	{
		ft_memcpy(new, *dst, len);
		free(*dst);
	}
	new[len] = c;
	new[len + 1] = '\0';
	*dst = new;
	return (1);
}

int	free_and_fail(char **p)
{
	if (*p)
	{
		free(*p);
		*p = NULL;
	}
	return (-1);
}

int	append_str(char **dst, const char *add)
{
	size_t	len;
	size_t	al;
	char	*new;

	if (!add)
		return (1);
	len = *dst ? ft_strlen(*dst) : 0;
	al = ft_strlen(add);
	new = malloc(len + al + 1);
	if (!new)
		return (free_and_fail(dst));
	if (*dst)
	{
		ft_memcpy(new, *dst, len);
		free(*dst);
	}
	ft_memcpy(new + len, add, al + 1);
	*dst = new;
	return (1);
}
