/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:57:33 by marvin            #+#    #+#             */
/*   Updated: 2025/08/13 20:57:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_env		*env;
	const char	*prompt;

	(void)argc;
	(void)argv;
	env = init_list(envp);
	set_env_value(&env, "?", "0");
	init_signals();
	prompt = ORANGE "🔥 minihell$ " RESET;
	while (1)
	{
		line = get_input_line(prompt);
		if (!line)
			break ;
		if (*line)
			process_command_line(line, &env);
		free(line);
	}
	free_env_list(env);
	return (0);
}