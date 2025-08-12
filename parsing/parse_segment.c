/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_segment.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <marvin@student.42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:10:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/12 14:10:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->redir_error = 0;
	cmd->next = NULL;
	cmd->cmd_separator = 0;
	return (cmd);
}

/* Retourne le nouveau segment, ou NULL en cas d'erreur */
t_cmd	*process_segment(char **tk, int *i, t_env *env)
{
	t_cmd	*newc;

	newc = init_cmd();
	if (!newc)
		return (NULL);
	newc->args = copy_args(tk, i, newc, env);
	if (!newc->args)
	{
		free(newc);
		return (NULL);
	}
	return (newc);
}
