/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 08:39:14 by zamohame          #+#    #+#             */
/*   Updated: 2025/08/14 09:33:32 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	run_parent_process(pid_t pid, int fd_in, int fd_out)
{
	int	status;

	g_signal_exit = 0;
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	waitpid(pid, &status, 0);
	if (g_signal_exit != 0)
		return (g_signal_exit);
		
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		return (128 + sig);
	}
	return (1);
}

int	setup_pipeline(t_cmd *cmd_list, t_pipeline *pipeline, t_env **env)
{
	pipeline->cmds = cmd_list_to_array(cmd_list, &pipeline->cmd_count);
	if (!pipeline->cmds)
		return (-1);
	pipeline->env = env;
	pipeline->pipes = NULL;
	if (pipeline->cmd_count == 1)
		return (execute_single_command(cmd_list, env));
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

void	execute_external_command(t_cmd *cmd, t_env **env)
{
	char	*path;
	char	**envp;

	envp = env_to_array(*env);
	if (!envp)
		exit(1);
	path = get_path(cmd->args[0], envp);
	if (!path)
	{
		print_error_message(cmd->args, 0, "Command '", "' not found");
		free_tab(envp);
		exit(127);
	}
	execve(path, cmd->args, envp);
	perror("execve");
	free_tab(envp);
	exit(1);
}
