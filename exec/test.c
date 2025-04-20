# include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
    char    **args;
    t_cmd   cmd;

    (void)argc;
    (void)argv;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break;
        args = split_args(line);
		printf("tu as 🫵 écrit : %s\n", line);
        cmd.args = args;
        if (args && args[0])
		    execute_command(&cmd, envp);
		free(line);
	}
	printf("exit\n");
	return (0);
}