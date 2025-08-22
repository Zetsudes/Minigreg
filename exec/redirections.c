/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:54:35 by marvin            #+#    #+#             */
/*   Updated: 2025/08/22 17:18:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_input_redirection(t_cmd *cmd, t_env *env, int *fd_in)
{
	if (cmd->heredoc)
	{
		*fd_in = heredoc_to_fd(cmd->infile, env);
		if (*fd_in < 0)
			return (1);
	}
	else if (cmd->infile)
	{
		*fd_in = open(cmd->infile, O_RDONLY);
		if (*fd_in < 0)
		{
			perror(cmd->infile);
			return (1);
		}
	}
	return (0);
}

int	handle_output_redirection(t_cmd *cmd, int fd_in, int *fd_out)
{
	if (cmd->outfile)
	{
		if (cmd->append)
			*fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			*fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*fd_out < 0)
		{
			perror(cmd->outfile);
			if (fd_in != STDIN_FILENO)
				close(fd_in);
			return (1);
		}
	}
	return (0);
}
