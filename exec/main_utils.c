
# include "../include/minishell.h"

char	*expand_buffer(char *line, size_t *cap, size_t *len, char c)
{
	if (*len + 1 >= *cap)
	{
	        if (*cap)
			*cap = *cap * 2;
		else
			*cap = 32;
		line = realloc(line, *cap);
		if (!line)
			return (NULL);
	}
	line[(*len)++] = c;
	return (line);
}

char	*read_line_noninteractive(void)
{
	char	*line;
	size_t	cap;
	size_t	len;
	char	c;
	ssize_t	r;

	line = NULL;
	cap = 0;
	len = 0;
	while ((r = read(STDIN_FILENO, &c, 1)) > 0)
	{
		if (c == '\n')
			break;
		line = expand_buffer(line, &cap, &len, c);
		if (!line)
			return (NULL);
	}
	if (r <= 0 && len == 0)
		return (free(line), NULL);
	if (line)
		line[len] = '\0';
	return (line);
}

void	process_command_line(char *line, t_env **env)
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

char	*get_input_line(const char *prompt)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		line = readline(prompt);
	else
	{
		ft_putstr_fd((char *)prompt, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		line = read_line_noninteractive();
	}
	return (line);
}