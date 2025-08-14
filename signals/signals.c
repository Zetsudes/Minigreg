/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 08:38:48 by zamohame          #+#    #+#             */
/*   Updated: 2025/08/14 10:48:11 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	handle_sigquit(int signum)
{
	(void)signum;
	write(1, "Quit (core dumped)\n", 19);
}

void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
