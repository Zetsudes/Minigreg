#include "../include/minishell.h"

void	print_cmd(t_cmd *cmd)
{
	for (int i = 0; cmd->args && cmd->args[i]; i++)
		printf("  arg[%d]: '%s'\n", i, cmd->args[i]);
	printf("  infile: '%s'\n", cmd->infile ? cmd->infile : "NULL");
	printf("  outfile: '%s'\n", cmd->outfile ? cmd->outfile : "NULL");
	printf("  append: %d\n", cmd->append);
	printf("  heredoc: %d\n", cmd->heredoc);
	printf("  cmdseparator: %d\n", cmd->cmd_separator);
	printf("\n");
}

static void	process_command_line(char *line, t_env **env)
{
	char	**tokens;
	t_cmd	*cmds;

	add_history(line);
	tokens = tokenize_line(line);
	if (!tokens)
	{
		ft_putendl_fd("minishell: syntax error", 2);
		set_env_value(env, "?", "2");
		return;
	}
	cmds = parse_tokens(tokens, *env);
	if (cmds)
	{
		execute_command_sequence(cmds, env);
		free_cmd_list(cmds);
	}
	else
		ft_putstr_fd("Parser returned NULL (syntax error?)\n", 2);
	free_tab(tokens);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_list(envp);
	set_env_value(&env, "?", "0");
	init_signals();
	while (1)
	{
		line = readline(ORANGE "🔥 minihell$ " RESET);
		if (!line)
			break ;
		if (*line)
			process_command_line(line, &env);
		free(line);
	}
	free(line);
	free_env_list(env);
	return (0);
}
