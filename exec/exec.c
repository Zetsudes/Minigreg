
# include "../include/minishell.h"

void	execute_command(t_cmd *cmd, char **envp)
{
	pid_t	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		return;
	}
	if (pid == 0)
	{
		execve(cmd->args[0], cmd->args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		int	status;
		waitpid(pid, &status, 0);
	}
}

char **split_args(char *line)
{
	if (!line)
		return (NULL);
	return (ft_split(line, ' '));
}