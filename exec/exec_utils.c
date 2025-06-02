
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

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Waits for all child processes to finish                <3
<3 Helper function used in execute_pipe() in pipe.c file  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void	wait_for_children(pid_t pids[], int cmd_count)
{
   int i;
   
   i = 0;
   while (i < cmd_count) 
   {
       int status; // Collects exit status of each child <3
       waitpid(pids[i], &status, 0);
       i++;
   }
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Initializes pipe data struct                           <3
<3 Helper function used in execute_pipe() in pipe.c file  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void	init_data(t_data *data, t_cmd *cmd)
{
    data->i = 0;
    data->cmd_count = count_commands(cmd); // Counts total commands in pipe <3
    data->num_pipes = data->cmd_count - 1; // One less pipe than commands <3
    data->current = cmd;
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Handles execution logic inside a child                 <3
<3 Helper function used in execute_pipe() in pipe.c file  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void	run_child_process(t_data *data, int pipes[][2], char **envp)
{
    child_process(pipes, data->i, data->cmd_count); // STDIN/STDOUT redirection <3
    close_pipes(pipes, data->num_pipes); // Closes all unused pipe ends <3
    if (is_builtin(data->current->args[0]))
    {
        handle_builtin(data->current, envp); // Executs builtin directly <3
        exit(0);
    }
    else
    {
        execute_external_command(data->current, envp); // Executes external with execve <3
        exit(1);
    }
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