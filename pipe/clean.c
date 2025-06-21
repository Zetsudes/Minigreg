#include "../include/minishell.h"

void	close_all_pipes(t_pipeline *pipeline)
{
	int	i;

	if (!pipeline->pipes)
		return ;
	i = 0;
	while (i < pipeline->cmd_count - 1)
	{
		if (pipeline->pipes[i])
		{
			close(pipeline->pipes[i][0]);
			close(pipeline->pipes[i][1]);
		}
		i++;
	}
}

void	cleanup_pipeline(t_pipeline *pipeline)
{
	int	i;

	if (pipeline->pipes)
	{
		i = 0;
		while (i < pipeline->cmd_count - 1)
		{
			if (pipeline->pipes[i])
			{
				close(pipeline->pipes[i][0]);
				close(pipeline->pipes[i][1]);
				free(pipeline->pipes[i]);
			}
			i++;
		}
		free(pipeline->pipes);
		pipeline->pipes = NULL;
	}
	if (pipeline->cmds)
	{
		i = 0;
		while (i < pipeline->cmd_count)
		{
			if (pipeline->cmds[i])
				free_tab(pipeline->cmds[i]);
					// or whatever function frees cmd[i]
			i++;
		}
		free(pipeline->cmds);
		pipeline->cmds = NULL;
	}
	// If you use heredocs in minishell and save them to a fixed file:
	// unlink(HEREDOC);
}

int	handle_fork_error(pid_t *pids, int i, t_pipeline *pipeline)
{
	perror("fork");
	while (--i >= 0)
		kill(pids[i], SIGTERM);
	cleanup_pipeline(pipeline);
	free(pids);
	return (1);
}
