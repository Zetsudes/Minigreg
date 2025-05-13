#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**args;
	char	**my_env;
	t_cmd	cmd;

	(void)argc;
	(void)argv;
	my_env = copy_env(envp);
	while (1)
	{
		line = readline("minihell$ ");
		if (!line)
			break ;
		args = split_args(line);
		cmd.args = args;
		cmd.cmd = args[0];
		if (args && args[0])
		{
			if (is_builtin(&cmd))
				handle_builtin(&cmd);
			else
				execute_command(&cmd, my_env);
		}
		add_history(line);
		free(line);
		free_tab(args);
	}
	free_tab(my_env);
	printf("exit\n");
	return (0);
}
