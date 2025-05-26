#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**args;
	t_cmd	cmd;
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_list(envp);
	print_lava_intro();
	while (1)
	{
		line = readline(ORANGE "🔥 minihell$ " RESET);
		if (!line)
			break ;
		args = split_args(line);
		cmd.args = args;
		cmd.cmd = args[0];
		if (args && args[0])
		{
			if (is_builtin(&cmd))
				handle_builtin(&cmd, &env);
			else
				execute_command(&cmd, env_to_array(env));
		}
		add_history(line);
		free(line);
		free_tab(args);
	}
	free_env_list(env);
	printf(RED "\n🔥 Fading back into the flames...\n\n" RESET);
	return (0);
}

