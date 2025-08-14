/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:30:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/14 09:32:21 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_sep(const char *s)
{
	if (!s)
		return (1);
	if (ft_strcmp(s, "|") == 0)
		return (1);
	if (ft_strcmp(s, ";") == 0)
		return (1);
	return (0);
}

char	*process_token(char *tok, t_env *env)
{
	char	*out;

	if (!tok)
		return (ft_strdup(""));
	out = expand_var(tok, env);
	if (!out)
		out = ft_strdup("");
	return (out);
}

void	free_args_partial(char **args, int k)
{
	while (k-- > 0)
		free(args[k]);
	free(args);
}

void	skip_to_sep(int *j, char **tokens)
{
	while (tokens[*j] && !is_sep(tokens[*j]))
		(*j)++;
}

void	add_piece(char **args, int *k, char *token, t_env *env)
{
	char	*tmp;

	tmp = process_token(token, env);
	if (tmp && (tmp[0] != '\0' || token_is_quoted(token)
			|| is_empty_quotes(token)))
	{
		args[*k] = tmp;
		*k += 1;
	}
	else
		free(tmp);
}
