/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <marvin@student.42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:30:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/12 14:30:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_after_segment(char **tk, int *i, t_cmd *cur)
{
	if (tk[*i] && !ft_strcmp(tk[*i], "|"))
	{
		*i += 1;
		if (handle_pipe_errors(tk, *i, cur))
			return (1);
		return (0);
	}
	if (tk[*i] && !ft_strcmp(tk[*i], ";"))
	{
		cur->cmd_separator = 1;
		*i += 1;
		if (!tk[*i] || !ft_strcmp(tk[*i], ";") || !ft_strcmp(tk[*i], "|"))
		{
			print_syntax_error(tk[*i]);
			return (1);
		}
		return (0);
	}
	if (tk[*i])
	{
		print_syntax_error(tk[*i]);
		return (1);
	}
	return (0);
}

static int	parse_tokens_loop(char **tk, t_env *env, t_cmd **head, t_cmd **cur)
{
	t_cmd	*newc;
	int		i;

	i = 0;
	while (tk[i])
	{
		newc = process_segment(tk, &i, env);
		if (!newc)
			return (free_cmd_list(*head), 1);
		if (!*head)
			*head = newc;
		else
			(*cur)->next = newc;
		*cur = newc;
		if (handle_after_segment(tk, &i, *cur))
			return (free_cmd_list(*head), 1);
	}
	return (0);
}

t_cmd	*parse_tokens(char **tk, t_env *env)
{
	t_cmd	*head;
	t_cmd	*cur;

	head = NULL;
	cur = NULL;
	if (!tk || !tk[0])
		return (NULL);
	if (!ft_strcmp(tk[0], "|"))
	{
		print_syntax_error("|");
		return (NULL);
	}
	if (parse_tokens_loop(tk, env, &head, &cur))
		return (NULL);
	return (head);
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = next;
	}
}
