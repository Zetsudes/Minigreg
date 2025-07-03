#include "../include/minishell.h"

void	close_all_pipes(t_pipeline *pipeline)
{
	int	i;

	if (!pipeline->pipes)
		return ;
	i = 0;
	while (i < pipeline->cmd_count - 1)
	{
		if (pipeline->pipes[i])
		{
			close(pipeline->pipes[i][0]);
			close(pipeline->pipes[i][1]);
		}
		i++;
	}
}

void    cleanup_pipeline(t_pipeline *p)
{
    int i;

    /* --- fermer & libérer les pipes --- */
    if (p->pipes)
    {
        i = 0;
        while (i < p->cmd_count - 1)
        {
            if (p->pipes[i])
            {
                close(p->pipes[i][0]);
                close(p->pipes[i][1]);
                free(p->pipes[i]);
            }
            i++;
        }
        free(p->pipes);
        p->pipes = NULL;
    }

    /* --- libérer seulement le tableau de pointeurs, PAS les t_cmd --- */
    free(p->cmds);
    p->cmds = NULL;
}

int	handle_fork_error(pid_t *pids, int i, t_pipeline *pipeline)
{
	perror("fork");
	while (--i >= 0)
		kill(pids[i], SIGTERM);
	cleanup_pipeline(pipeline);
	free(pids);
	return (1);
}
