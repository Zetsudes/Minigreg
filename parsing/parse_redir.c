/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:17:48 by marvin            #+#    #+#             */
/*   Updated: 2025/07/21 08:17:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>

static void	set_in_redir(t_cmd *cmd, char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		cmd->redir_error = 1;
	}
	else
		close(fd);
	free(cmd->infile);
	cmd->infile = ft_strdup(file);
	cmd->heredoc = 0;
}

static void	set_heredoc(t_cmd *cmd, char *file)
{
	free(cmd->infile);
	cmd->heredoc = 1;
	cmd->infile = ft_strdup(file);
}

static void	set_out_trunc(t_cmd *cmd, char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(file);
		cmd->redir_error = 1;
	}
	else
		close(fd);
	free(cmd->outfile);
	cmd->append = 0;
	cmd->outfile = ft_strdup(file);
}

static void	set_out_append(t_cmd *cmd, char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(file);
		cmd->redir_error = 1;
	}
	else
		close(fd);
	free(cmd->outfile);
	cmd->append = 1;
	cmd->outfile = ft_strdup(file);
}

int	set_redirection(t_cmd *cmd, char *op, char *file)
{
	if (!ft_strcmp(op, "<"))
		set_in_redir(cmd, file);
	else if (!ft_strcmp(op, "<<"))
		set_heredoc(cmd, file);
	else if (!ft_strcmp(op, ">"))
		set_out_trunc(cmd, file);
	else if (!ft_strcmp(op, ">>"))
		set_out_append(cmd, file);
	if ((op[0] == '<' && !cmd->infile) || (op[0] == '>' && !cmd->outfile))
		return (0);
	return (1);
}
