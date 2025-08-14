/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:51:51 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 10:31:52 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Executes single command with/without pipes <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	execute_single_command(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	int		fd_in;
	int		fd_out;
	int		setup_result;

	setup_result = setup_and_validate_command(cmd, &fd_in, &fd_out);
	if (setup_result != -1)
		return (setup_result);
	if (is_builtin(cmd))
		return (execute_builtin(cmd, env, fd_in, fd_out));
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		return (1);
	}
	if (pid == 0)
		run_child_process(cmd, env, fd_in, fd_out);
	return (run_parent_process(pid, fd_in, fd_out));
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3
<3 Executes a pipeline <3
<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	execute_commands(t_cmd *cmd_list, t_env **env)
{
	t_pipeline	pipeline;
	pid_t		*pids;
	int			result;

	if (!cmd_list)
		return (1);
	result = setup_pipeline(cmd_list, &pipeline, env);
	if (result != 999)
		return (result);
	pids = init_pipeline_resources(&pipeline);
	if (!pids)
		return (1);
	if (execute_commands_pipeline(&pipeline, pids) != 0)
		return (1);
	close_all_pipes(&pipeline);
	result = wait_for_children(&pipeline, pids);
	cleanup_pipeline(&pipeline);
	free(pids);
	return (result);
}

/*
<3<3<3<3<3<3<3<3<3<3
<3 Runs a builtin <3
<3<3<3<3<3<3<3<3<3<3
*/
int	execute_builtin(t_cmd *cmd, t_env **env, int fd_in, int fd_out)
{
	int	saved[2];
	int	result;

	save_fds(fd_in, fd_out, saved);
	result = handle_builtin(cmd, env);
	if (saved[0] != -1)
	{
		dup2(saved[0], STDIN_FILENO);
		close(saved[0]);
	}
	if (saved[1] != -1)
	{
		dup2(saved[1], STDOUT_FILENO);
		close(saved[1]);
	}
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	return (result);
}

/*
<3<3<3<3<3<3<3<3<3<3
<3 Child process <3
<3<3<3<3<3<3<3<3<3<3
*/
void	run_child_process(t_cmd *cmd, t_env **env, int fd_in, int fd_out)
{
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
	exec_command(cmd, env);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3
<3 Executes sequences <3
<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	execute_command_sequence(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*next;
	t_cmd	*after;
	int		status;

	status = 0;
	while (cmd_list)
	{
		next = cmd_list;
		while (next->next && next->cmd_separator == 0)
			next = next->next;
		after = next->next;
		next->next = NULL;
		status = process_command_group(cmd_list, env);
		next->next = after;
		cmd_list = after;
	}
	return (status);
}
