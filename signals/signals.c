#include "../include/minishell.h"

void	handle_sigint(int signum)
{
	static int hits = 0;
	(void)signum;
	hits++;
	printf("\nSIGINT #%d\n", hits);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}


void	init_signals(void)
{
	signal(SIGINT, handle_sigint);  // Ctrl-C
	signal(SIGQUIT, SIG_IGN);       // Ctrl-backslash
}

static int  *store(void)
{
    static int  code = 0;
    return (&code);
}

int get_exit_status(void)
{
    return (*store());
}

void set_exit_status(int c)
{
    *store() = c;
}