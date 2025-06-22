
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
	int		status;

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	if (handle_input_redirection(cmd, &fd_in))
		return (1);
	if (handle_output_redirection(cmd, fd_in, &fd_out))
		return (1);
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
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
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
