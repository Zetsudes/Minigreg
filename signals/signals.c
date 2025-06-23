#include "../include/minishell.h"

void	handle_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	init_signals(void)
{
	signal(SIGINT, handle_sigint);  // Ctrl-C
	signal(SIGQUIT, SIG_IGN);       // Ctrl-backslash
}