
# include "../include/minishell.h"

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int setup_pipeline(t_cmd *cmd_list, t_pipeline *pipeline, t_env **env)
{
    pipeline->cmds = cmd_list_to_array(cmd_list, &pipeline->cmd_count);
    if (!pipeline->cmds)
        return (-1);
    
    pipeline->env = env;
    pipeline->pipes = NULL;
    
    // Single command - no pipes needed
    if (pipeline->cmd_count == 1)
    {
        if (is_builtin(cmd_list))
            return (handle_builtin(cmd_list, env));
        else
            return (execute_single_command(cmd_list, env));
    }
    return (999);
}

// Helper function to initialize pipes and allocate pids
pid_t *init_pipeline_resources(t_pipeline *pipeline)
{
    pid_t *pids;
    
    if (init_pipeline_pipes(pipeline) == -1)
    {
        free(pipeline->cmds);
        return (NULL);
    }
    
    pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
    if (!pids)
    {
        cleanup_pipeline(pipeline);
        return (NULL);
    }
    return (pids);
}

// Helper function to execute all commands in the pipeline
int execute_all_commands(t_pipeline *pipeline, pid_t *pids)
{
    int fd_in;
    int i;
    
    fd_in = STDIN_FILENO;
    i = 0;
    while (i < pipeline->cmd_count)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            // Kill any previously started processes
            while (--i >= 0)
                kill(pids[i], SIGTERM);
            cleanup_pipeline(pipeline);
            free(pids);
            return (1);
        }
        
        if (pids[i] == 0) // Child process
        {
            execute_pipeline_command(pipeline, i, fd_in, pipeline->env);
            exit(1); // Should never reach here
        }
        
        // Parent: manage file descriptors
        if (i > 0 && fd_in != STDIN_FILENO)
            close(fd_in);
        if (i < pipeline->cmd_count - 1)
        {
            close(pipeline->pipes[i][1]);
            fd_in = pipeline->pipes[i][0];
        }
        i++;
    }
    return (0);
}

// Helper function to wait for all children and get exit status
int wait_for_children(t_pipeline *pipeline, pid_t *pids)
{
    int status;
    int exit_status;
    int i;
    
    exit_status = 0;
    i = 0;
    while (i < pipeline->cmd_count)
    {
        waitpid(pids[i], &status, 0);
        if (i == pipeline->cmd_count - 1) // Last command's exit status
        {
            if (WIFEXITED(status))
                exit_status = WEXITSTATUS(status);
            else
                exit_status = 1;
        }
        i++;
    }
    return (exit_status);
}

void	print_lava_intro(void)
{
	printf(RED "\n");
	printf("     🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n");
	printf(ORANGE "        WELCOME TO THE DEPTHS OF MINIHELL\n");
	printf(RED "     🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n\n");

	printf(RED "               (  .      )\n");
	printf(ORANGE "          )           (              )\n");
	printf(YELLOW "                .  '   .   '  .  '  .\n");
	printf(RED "     (    , )       (.   )  (   ',    )\n");
	printf(ORANGE "      .' ) ( . )    ,  ( ,     )   ( .\n");
	printf(RED "    ) ( . ) ( . )  ) _ )     )  ( ) . )\n");
	printf(RED "           🔥   🔥   🔥   🔥   🔥\n");
	printf(RESET "\n");
}