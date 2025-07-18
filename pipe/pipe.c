
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
		if (!pipeline->pipes[i])
		{
			while (--i >= 0)
				free(pipeline->pipes[i]);
			free(pipeline->pipes);
			return (-1);
		}
		if (pipe(pipeline->pipes[i]) == -1)
		{
			free(pipeline->pipes[i]);
			while (--i >= 0)
			{
				close(pipeline->pipes[i][0]);
				close(pipeline->pipes[i][1]);
				free(pipeline->pipes[i]);
			}
			free(pipeline->pipes);
			return (-1);
		}
		i++;
	}
	return (0);
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
void	execute_pipeline_command(t_pipeline *pipeline, int cmd_index, int fd_in,
		t_env **env)
{
	t_cmd	*cmd;
	int		fd_out;
	char	*path;
	char	**envp;

	cmd = pipeline->cmds[cmd_index];
	if (cmd->redir_error)
                exit(1);
	// Set up input
	if (cmd->heredoc) // Using your teammate's heredoc flag
		fd_in = handle_heredoc(cmd);
	else if (cmd->infile) // Handle input redirection
	{
		fd_in = open(cmd->infile, O_RDONLY);
		if (fd_in < 0)
		{
			perror(cmd->infile);
			exit(1);
		}
	}
	// Set up output
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
			}
			else if (cmd_index < pipeline->cmd_count - 1)
					fd_out = pipeline->pipes[cmd_index][1];
			else
					fd_out = STDOUT_FILENO;
	// Redirect file descriptors
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
	// Close all pipe fds in child
	close_all_pipes(pipeline);
	// Execute command
	if (is_builtin(cmd))
	{
		exit(handle_builtin(cmd, env));
	}
	else
	{
		envp = env_to_array(*env);
		if (!envp)
			exit(1);
		path = get_path(cmd->args[0], envp);
		if (!path)
		{
			ft_putstr_fd("Command '", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd("' not found\n", 2);
			free_tab(envp);
			exit(127);
		}
		execve(path, cmd->args, envp);
		perror("execve");
		free_tab(envp);
		exit(1);
	}
}
