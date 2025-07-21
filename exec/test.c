#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/minishell.h"

static char *read_line_noninteractive(void)
{
    char *line = NULL;
    size_t cap = 0;
    size_t len = 0;
    char c;
    ssize_t r;

    while ((r = read(STDIN_FILENO, &c, 1)) > 0)
    {
        if (c == '\n')
            break;
        if (len + 1 >= cap)
        {
            cap = cap ? cap * 2 : 32;
            line = realloc(line, cap);
            if (!line)
                return NULL;
        }
        line[len++] = c;
    }
    if (r <= 0 && len == 0)
        return free(line), NULL;
    if (line)
        line[len] = '\0';
    return line;
}

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
        const char *prompt = ORANGE "🔥 minihell$ " RESET;
        while (1)
        {
                if (isatty(STDIN_FILENO))
                        line = readline(prompt);
                else
                {
                        ft_putstr_fd((char *)prompt, STDOUT_FILENO);
                        ft_putchar_fd('\n', STDOUT_FILENO);
                        line = read_line_noninteractive();
                }
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
