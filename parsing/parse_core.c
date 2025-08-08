/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:16:25 by marvin            #+#    #+#             */
/*   Updated: 2025/07/21 08:16:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*parse_tokens(char **tk, t_env *env)
{
	t_cmd	*head;
	t_cmd	*cur;
	int		i;

	head = NULL;
	cur = NULL;
	i = 0;
	if (!tk || !tk[0])
		return (NULL);
	if (!ft_strcmp(tk[0], "|"))
		return (print_syntax_error("|"), NULL);
	while (tk[i])
	{
		if (process_segment(&head, &cur, tk, &i, env))
			return (NULL);
		if (tk[i] && !ft_strcmp(tk[i], "|"))
		{
			i++;
			if (handle_pipe_errors(tk, i, cur))
				return (free_cmd_list(head), NULL);
		}
		else if (tk[i] && !ft_strcmp(tk[i], ";"))
		{
			cur->cmd_separator = 1;
			i++;
			if (!tk[i] || !ft_strcmp(tk[i], ";")
				|| !ft_strcmp(tk[i], "|"))
				return (print_syntax_error(tk[i]),
					free_cmd_list(head), NULL);
		}
		else if (tk[i])
			return (print_syntax_error(tk[i]),
				free_cmd_list(head), NULL);
	}
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
