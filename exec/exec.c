
#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Executes commands with / without pipes <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/

int	execute_single_command(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	int		fd_in;
	int		fd_out;

	if (cmd->redir_error)
                return (1);
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	if (handle_input_redirection(cmd, &fd_in))
			return (1);
	if (handle_output_redirection(cmd, fd_in, &fd_out))
			return (1);
	if (!cmd->args || !cmd->args[0])
	{
			if (fd_in != STDIN_FILENO)
					close(fd_in);
			if (fd_out != STDOUT_FILENO)
					close(fd_out);
			return (0);
	}
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

int	execute_builtin(t_cmd *cmd, t_env **env, int fd_in, int fd_out)
{
	int	saved[2];
	int	result;

	saved[0] = -1;
	saved[1] = -1;
	if (fd_in != STDIN_FILENO && (saved[0] = dup(STDIN_FILENO)) != -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out != STDOUT_FILENO && (saved[1] = dup(STDOUT_FILENO)) != -1)
		dup2(fd_out, STDOUT_FILENO);
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

static void	update_dollar_question(t_cmd *cmd_list, char *exit_code)
{
	t_cmd	*current;
	int		i;
	char	*new_arg;
	
	current = cmd_list;
	while (current)
	{
		i = 0;
		while (current->args && current->args[i])
		{
			// Check if arg is exactly "?"
			if (ft_strcmp(current->args[i], "?") == 0)
			{
				free(current->args[i]);
				current->args[i] = ft_strdup(exit_code);
			}
			// Check if arg starts with "?" (like "?HELLO")
			else if (current->args[i][0] == '?')
			{
				// Replace ? with exit_code at the beginning
				new_arg = malloc(strlen(exit_code) + strlen(current->args[i]));
				if (new_arg)
				{
					strcpy(new_arg, exit_code);
					strcat(new_arg, current->args[i] + 1); // Skip the '?'
					free(current->args[i]);
					current->args[i] = new_arg;
				}
			}
			i++;
		}
		current = current->next;
	}
}

static void	expand_command_args(t_cmd *cmd, t_env *env)
{
	int		i;
	char	*expanded;
	
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		// Only expand if it contains $ but skip pure $? (already handled)
		if (ft_strchr(cmd->args[i], '$') && ft_strcmp(cmd->args[i], "$?") != 0)
		{
			expanded = expand_var(cmd->args[i], env);
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
}

int	execute_command_sequence(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*next;
	t_cmd	*after;
	t_cmd	*current;
	int		status;
	char	*exit_code;
	char	*status_str;
	
	status = 0;
	while (cmd_list)
	{
		next = cmd_list;
		while (next->next && next->cmd_separator == 0)
			next = next->next;
		after = next->next;
		next->next = NULL;
		
		// Get the CURRENT exit code BEFORE doing anything
		exit_code = get_env_value(*env, "?");
		if (!exit_code)
			exit_code = "0";
		// First update $? with current exit code
		update_dollar_question(cmd_list, exit_code);
		
		// Then expand other variables
		current = cmd_list;
		while (current)
		{
			expand_command_args(current, *env);
			current = current->next;
		}
		
		// Execute the commands
		status = execute_commands(cmd_list, env);
		
		// Update exit code for next iteration
		status_str = ft_itoa(status);
		if (status_str)
		{
			set_env_value(env, "?", status_str);
			free(status_str);
		}
		
		next->next = after;
		cmd_list = after;
	}
	return (status);
}
