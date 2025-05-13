
# include "../include/minishell.h"

void	execute_command(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	char	*path;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}
	if (pid == 0)
	{
		path = get_path(cmd->args[0], envp);
		if (!path)
		{
			ft_putstr_fd("Command '", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd("' not found\n", 2);
			exit(127);
		}
		execve(path, cmd->args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
}

char **split_args(char *line)
{
	if (!line)
		return (NULL);
	return (ft_split(line, ' '));
}