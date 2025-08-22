/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:49:12 by marvin            #+#    #+#             */
/*   Updated: 2025/08/22 17:18:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	validate_path(char *path, char **envp)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
	{
		print_error_message_str(path, "", ": is a directory");
		cleanup_and_exit(envp, path, 126);
	}
	if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
	{
		print_error_message_str(path, "", ": Permission denied");
		cleanup_and_exit(envp, path, 126);
	}
}

int	setup_and_validate_command(t_cmd *cmd, t_env *env, int *fd_in, int *fd_out)
{
	if (cmd->redir_error)
		return (1);
	*fd_in = STDIN_FILENO;
	*fd_out = STDOUT_FILENO;
	if (handle_input_redirection(cmd, env, fd_in))
		return (1);
	if (handle_output_redirection(cmd, *fd_in, fd_out))
		return (1);
	if (!cmd->args || !cmd->args[0])
	{
		ft_putendl_fd("minishell: command not found", 2);
		if (*fd_in != STDIN_FILENO)
			close(*fd_in);
		if (*fd_out != STDOUT_FILENO)
			close(*fd_out);
		return (0);
	}
	return (-1);
}

int	process_command_group(t_cmd *cmd_list, t_env **env)
{
	char	*exit_code;
	char	*status_str;
	int		status;

	exit_code = get_env_value(*env, "?");
	if (!exit_code)
		exit_code = "0";
	status = execute_commands(cmd_list, env);
	status_str = ft_itoa(status);
	if (status_str)
	{
		set_env_value(env, "?", status_str);
		free(status_str);
	}
	return (status);
}

void	save_fds(int fd_in, int fd_out, int saved[2])
{
	saved[0] = -1;
	saved[1] = -1;
	if (fd_in != STDIN_FILENO)
	{
		saved[0] = dup(STDIN_FILENO);
		if (saved[0] != -1)
			dup2(fd_in, STDIN_FILENO);
	}
	if (fd_out != STDOUT_FILENO)
	{
		saved[1] = dup(STDOUT_FILENO);
		if (saved[1] != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
}
