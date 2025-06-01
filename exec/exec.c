
# include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Executes command with fork and execve  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void	execute_command(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	char	*path;
	int status;

	pid = fork(); // Creates child process <3
	if (pid == -1)
	{
		perror("fork");
		return;
	}
	if (pid == 0) // Child process <3
	{
		path = get_path(cmd->args[0], envp); // Finds full path to the command <3
		if (!path)
		{
			ft_putstr_fd("Command '", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd("' not found\n", 2);
			exit(127); // Command not found exit code <3
		}
		execve(path, cmd->args, envp); // Executes command <3
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else // Parent process <3
		waitpid(pid, &status, 0); // Waits for child to finish <3
}

char **split_args(char *line)
{
	if (!line)
		return (NULL);
	return (ft_split(line, ' '));
}