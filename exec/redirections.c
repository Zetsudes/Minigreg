
#include "../include/minishell.h"

int	handle_heredoc(t_cmd *cmd)
{
	int		pipe_fd[2];
	char	*line;
	char	*limiter;

	limiter = cmd->infile;
	if (pipe(pipe_fd) == -1)
		return (-1);
	ft_putstr_fd("heredoc> ", 1);
	line = get_next_line(0);
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& (line[ft_strlen(limiter)] == '\n'
				|| line[ft_strlen(limiter)] == '\0'))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(0);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	handle_input_redirection(t_cmd *cmd, int *fd_in)
{
	if (cmd->heredoc)
	{
		*fd_in = handle_heredoc(cmd);
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
