
#include "../include/minishell.h"

int	setup_pipeline(t_cmd *cmd_list, t_pipeline *pipeline, t_env **env)
{
	pipeline->cmds = cmd_list_to_array(cmd_list, &pipeline->cmd_count);
	if (!pipeline->cmds)
		return (-1);
	pipeline->env = env;
	pipeline->pipes = NULL;
	if (pipeline->cmd_count == 1)
	{
		if (is_builtin(cmd_list))
			return (handle_builtin(cmd_list, env));
		else
			return (execute_single_command(cmd_list, env));
	}
	return (999);
}

pid_t	*init_pipeline_resources(t_pipeline *pipeline)
{
	pid_t	*pids;

	if (init_pipes(pipeline) == -1)
	{
		free(pipeline->cmds);
		return (NULL);
	}
	pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pids)
	{
		cleanup_pipeline(pipeline);
		return (NULL);
	}
	return (pids);
}

void	update_fds(int *fd_in, t_pipeline *pipeline, int i)
{
	if (i > 0 && *fd_in != STDIN_FILENO)
		close(*fd_in);
	if (i < pipeline->cmd_count - 1)
	{
		close(pipeline->pipes[i][1]);
		*fd_in = pipeline->pipes[i][0];
	}
}
