/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:57:33 by marvin            #+#    #+#             */
/*   Updated: 2025/08/26 11:02:37 by zamohame         ###   ########.fr       */
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
		if (g_signal_exit != 0)
		{
			set_env_value(&env, "?", ft_itoa(g_signal_exit));
			g_signal_exit = 0;
		}
		if (!line)
			break ;
		if (*line)
			process_command_line(line, &env);
		free(line);
	}
	free_env_list(env);
}
