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

void	cleanup_pipeline(t_pipeline *p)
{
	int i;

	if (p->pipes)
	{
		i = 0;
		while (i < p->cmd_count - 1)
		{
			if (p->pipes[i])
			{
				close(p->pipes[i][0]);
				close(p->pipes[i][1]);
				free(p->pipes[i]);
			}
			i++;
		}
		free(p->pipes);
		p->pipes = NULL;
	}
	free(p->cmds);
	p->cmds = NULL;
}

void	cleanup_partial_pipes(t_pipeline *pipeline, int allocated_count)
{
	int	i;

	i = 0;
	while (i < allocated_count)
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

void	cleanup_and_exit(char **envp, char *path, int exit_code)
{
	free_tab(envp);
	if (path)
		free(path);
	exit(exit_code);
}