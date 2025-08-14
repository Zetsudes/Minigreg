
# include "../include/minishell.h"

int	init_pipes(t_pipeline *pipeline)
{
	int	i;

	if (pipeline->cmd_count <= 1)
		return (0);
	pipeline->pipes = malloc(sizeof(int *) * (pipeline->cmd_count - 1));
	if (!pipeline->pipes)
		return (-1);
	i = 0;
	while (i < pipeline->cmd_count - 1)
	{
		pipeline->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipeline->pipes[i] || pipe(pipeline->pipes[i]) == -1)
		{
			if (pipeline->pipes[i])
				free(pipeline->pipes[i]);
			cleanup_partial_pipes(pipeline, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	setup_input_fd(t_cmd *cmd, int fd_in)
{
	if (cmd->heredoc)
		return (handle_heredoc(cmd));
	else if (cmd->infile)
	{
		fd_in = open(cmd->infile, O_RDONLY);
		if (fd_in < 0)
		{
			perror(cmd->infile);
			exit(1);
		}
	}
	return (fd_in);
}

int	setup_output_fd(t_cmd *cmd, int cmd_index, t_pipeline *pipeline)
{
	int	fd_out;

	if (cmd->outfile)
	{
		if (cmd->append)
			fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out < 0)
		{
			perror(cmd->outfile);
			exit(1);
		}
		return (fd_out);
	}
	else if (cmd_index < pipeline->cmd_count - 1)
		return (pipeline->pipes[cmd_index][1]);
	else
		return (STDOUT_FILENO);
}

int	execute_commands_pipeline(t_pipeline *pipeline, pid_t *pids)
{
	int	fd_in;
	int	i;

	fd_in = STDIN_FILENO;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (handle_fork_error(pids, i, pipeline));
		if (pids[i] == 0)
			execute_pipeline_command(pipeline, i, fd_in, pipeline->env);
		update_fds(&fd_in, pipeline, i);
		i++;
	}
	return (0);
}

// Execute individual command in pipeline
void	execute_pipeline_command(t_pipeline *pipeline, int cmd_index, int fd_in, t_env **env)
{
	t_cmd	*cmd;
	int		fd_out;

	cmd = pipeline->cmds[cmd_index];
	if (cmd->redir_error)
		exit(1);
	fd_in = setup_input_fd(cmd, fd_in);
	fd_out = setup_output_fd(cmd, cmd_index, pipeline);
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	close_all_pipes(pipeline);
	if (is_builtin(cmd))
		exit(handle_builtin(cmd, env));
	execute_external_command(cmd, env);
}