/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:52:50 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 09:28:49 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* assure qu'il y a au moins 1 octet libre pour ajouter un char */
static char	*ensure_capacity(char *line, size_t *cap, size_t len)
{
	size_t	new_cap;
	char	*new_line;

	if (len + 1 < *cap)
		return (line);
	if (*cap)
		new_cap = (*cap) * 2;
	else
		new_cap = 32;
	new_line = malloc(new_cap);
	if (!new_line)
	{
		free(line);
		return (NULL);
	}
	if (line && len > 0)
		ft_memcpy(new_line, line, len);
	free(line);
	*cap = new_cap;
	return (new_line);
}

char	*expand_buffer(char *line, size_t *cap, size_t *len, char c)
{
	line = ensure_capacity(line, cap, *len);
	if (!line)
		return (NULL);
	line[*len] = c;
	*len += 1;
	line[*len] = '\0';
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
	while (1)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r <= 0 || c == '\n')
			break ;
		line = expand_buffer(line, &cap, &len, c);
		if (!line)
			return (NULL);
	}
	if (r <= 0 && len == 0)
	{
		free(line);
		return (NULL);
	}
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
		return ;
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
