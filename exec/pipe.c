
#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Well this one is pretty obvious isn't it <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int count_commands(t_cmd *cmd)
{
    int count;

    count = 0;
    while(cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return (count);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Well this one is also pretty obvious isn't it  <3
<3 Helper function used in execute_pipe() below   <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int create_pipes(int pipes[][2], int num_pipes)
{
    int i ;
    
    i = 0;
    while (i < num_pipes) 
    {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return (-1);
        }
        i++;
    }
    return (0);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Well this one is also pretty obvious isn't it  <3
<3 Helper function used in execute_pipe() below   <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void    close_pipes(int pipes[][2], int num_pipes)
{
    int i;
    
    i = 0;
    while (i < num_pipes) 
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 STDIN/STDOUT redirection in child and closes fds                 <3
<3 Helper function used in run_child_process() in exec_utils.c file <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void    child_process(int pipes[][2], int index_cmd, int total_cmd, int num_pipes)
{
    int i;

    if (index_cmd > 0)
        dup2(pipes[index_cmd - 1][0], STDIN_FILENO); // Gets input from previous pipe <3
    if (index_cmd < total_cmd - 1)
        dup2(pipes[index_cmd][1], STDOUT_FILENO); // Sends output to next pipe <3
    i = 0;
    while (i < num_pipes) 
    {
        close(pipes[i][0]); // Closes all pipe ends after redirection <3
        close(pipes[i][1]);
        i++;
    }
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Executes a pipeline of commands in order <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void execute_pipe(t_cmd *cmd, char **envp)
{
    t_data  data;
    pid_t   pids[data->cmd_count];
    int         pipes[data->num_pipes][2];

    init_data(&data, cmd);
    if (create_pipes(pipes, data.num_pipes) == -1)
        return;
    while (data.i < data.cmd_count)
    {
        pids[data.i] = fork();
        if (pids[data.i] == -1)
        {
            perror("fork");
            return;
        }
        if (pids[data.i] == 0)
            run_child_process(&data, pipes, envp);
        data.current = data.current->next;
        data.i++;
    }
    close_pipes(pipes, data.num_pipes);
    wait_for_children(pids, data.cmd_count);
}
