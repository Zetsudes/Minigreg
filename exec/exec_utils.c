#include "../include/minishell.h"
#include <errno.h>

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

t_cmd	**cmd_list_to_array(t_cmd *cmd_list, int *count)
{
	t_cmd	**cmd_array;
	t_cmd	*curr;
	int	i;

	*count = 0;
	curr = cmd_list;
	while (curr)
	{
		(*count)++;
		curr = curr->next;
	}
	cmd_array = malloc(sizeof(t_cmd *) * (*count + 1));
	if (!cmd_array)
		return (NULL);
	curr = cmd_list;
	i = 0;
	while (curr)
	{
		cmd_array[i] = curr;
		curr = curr->next;
		i++;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}

static void	handle_execve_error(t_cmd *cmd, char **envp)
{
	if (errno == ENOEXEC)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": Permission denied", 2);
		free_tab(envp);
		exit(126);
	}
	else if (errno == EACCES || errno == EISDIR)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		free_tab(envp);
		exit(127);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	free_tab(envp);
	exit(1);
}

void	exec_command(t_cmd *cmd, t_env **env)
{
	char	**envp;
	char	*path;

	envp = env_to_array(*env);
	if (!envp)
		exit(1);
	path = get_path(cmd->args[0], envp);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		free_tab(envp);
		exit(127);
	}
	if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
	{
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Permission denied", 2);
		free_tab(envp);
		exit(126);
	}
	execve(path, cmd->args, envp);
	handle_execve_error(cmd, envp);
}

int	wait_for_children(t_pipeline *pipeline, pid_t *pids)
{
	int	status;
	int	exit_status;
	int	i;

	exit_status = 0;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pipeline->cmd_count - 1)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else
				exit_status = 1;
		}
		i++;
	}
	return (exit_status);
}
