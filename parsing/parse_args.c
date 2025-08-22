/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:30:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/22 17:34:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_redirection(char **tokens, int *j, t_cmd *cmd, t_env *env)
{
	char	*op;
	char	*file;
	char	*fname;

	op = tokens[*j];
	file = tokens[*j + 1];
	if (!file || is_token_operator(file))
	{
		print_syntax_error(file);
		return (0);
	}
	fname = process_token(file, env);
	if (!fname || !set_redirection(cmd, op, fname))
	{
		free(fname);
		return (0);
	}
	free(fname);
	*j += 2;
	return (1);
}

int	count_effective_args(char **tk, int j, t_env *env)
{
	int		count;
	char	*tmp;

	count = 0;
	while (tk[j] && !is_sep(tk[j]))
	{
		if (is_token_operator(tk[j]))
		{
			if (!tk[j + 1])
				break ;
			j += 2;
			continue ;
		}
		tmp = process_token(tk[j], env);
		if (tmp && (tmp[0] != '\0' || token_is_quoted(tk[j])
				|| is_empty_quotes(tk[j])))
			count++;
		free(tmp);
		j++;
	}
	return (count);
}

int	token_is_quoted(const char *s)
{
	char		q;
	const char	*start;

	while (*s)
	{
		if (*s == '\'' || *s == '"')
		{
			q = *s;
			s++;
			start = s;
			while (*s && *s != q)
				s++;
			if (*s == q && s - start > 0)
				return (1);
		}
		else
			s++;
	}
	return (0);
}

static int	handle_redir_token(char **tokens, int *j, t_cmd *cmd, t_env *env)
{
	if (!handle_redirection(tokens, j, cmd, env))
		return (0);
	if (cmd->redir_error)
		skip_to_sep(j, tokens);
	return (1);
}

char	**copy_args(char **tokens, int *i, t_cmd *cmd, t_env *env)
{
	int		j;
	int		k;
	char	**args;

	j = *i;
	args = malloc(sizeof(char *) * (count_effective_args(tokens, j, env) + 1));
	if (!args)
		return (NULL);
	k = 0;
	while (tokens[j] && !is_sep(tokens[j]))
	{
		if (is_token_operator(tokens[j]))
		{
			if (!handle_redir_token(tokens, &j, cmd, env))
				return (free_args_partial(args, k), NULL);
			continue ;
		}
		add_piece(args, &k, tokens[j], env);
		j++;
	}
	args[k] = NULL;
	*i = j;
	return (args);
}
