/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:18:53 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 09:33:00 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_token_operator(const char *token)
{
	if (!token)
		return (0);
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>")
		|| !ft_strcmp(token, "|") || !ft_strcmp(token, ";"));
}

int	count_args(char **tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i] && tokens[i][0] != '|' && tokens[i][0] != ';'
		&& tokens[i][0] != '<' && tokens[i][0] != '>')
	{
		count++;
		i++;
	}
	return (count);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (1);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
