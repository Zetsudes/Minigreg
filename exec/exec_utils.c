/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:57:19 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 09:28:36 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	int		i;

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
		cmd_array[i++] = curr;
		curr = curr->next;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}

void	handle_execve_error(t_cmd *cmd, char **envp)
{
	if (errno == EACCES)
	{
		print_error_message(cmd->args, 0, "", ": Permission denied");
		free_tab(envp);
		exit(126);
	}
	else if (errno == ENOEXEC)
	{
		free_tab(envp);
		exit(0);
	}
	print_error_message(cmd->args, 0, "minishell: ", ": ");
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
		print_error_message_str(cmd->args[0], "", ": command not found");
		cleanup_and_exit(envp, NULL, 127);
	}
	validate_path(path, envp);
	execve(path, cmd->args, envp);
	handle_execve_error(cmd, envp);
}

int wait_for_children(t_pipeline *pipeline, pid_t *pids)
{
	int	status;
	int	exit_status;
	int	i;

	g_signal_exit = 0;
	exit_status = 0;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pipeline->cmd_count - 1)
			exit_status = get_exit_status(status);
		i++;
	}
	return (exit_status);
}
