
# include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Executes command with fork and execve  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/

void	execute_command(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int status;

	envp = env_to_array(*env);
	if (!envp)
		return;

	pid = fork(); // Creates child process <3
	if (pid == -1)
	{
		perror("fork");
		free_tab(envp);
		return;
	}
	if (pid == 0) // Child process <3
	{
		path = get_path(cmd->args[0], envp); // Finds full path to the command <3
		if (!path)
		{
			ft_putstr_fd("Command '", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd("' not found\n", 2);
			free_tab(envp);
			exit(127); // Command not found exit code <3
		}
		execve(path, cmd->args, envp); // Executes command <3
		perror("execve");
		free_tab(envp);
		exit(EXIT_FAILURE);
	}
	else // Parent process <3
	{
		free_tab(envp);
		waitpid(pid, &status, 0); // Waits for child to finish <3
	}
}

t_cmd **cmd_list_to_array(t_cmd *cmd_list, int *count)
{
    t_cmd   **cmd_array;
    t_cmd   *curr;
    int     i;

    *count = 0;
    curr = cmd_list;
    while (curr)  // Count commands
    {
        (*count)++;
        curr = curr->next;
    }
    cmd_array = malloc(sizeof(t_cmd*) * (*count + 1));
    if (!cmd_array)
        return (NULL);
    curr = cmd_list;
    i = 0;
    while (curr)  // Fill array
    {
        cmd_array[i] = curr;
        curr = curr->next;
        i++;
    }
    cmd_array[i] = NULL;
    return (cmd_array);
}

// Initialize pipes (adapted from your pipex)
int init_pipeline_pipes(t_pipeline *pipeline)
{
    int i;
    
    if (pipeline->cmd_count <= 1)
        return (0);
        
    pipeline->pipes = malloc(sizeof(int*) * (pipeline->cmd_count - 1));
    if (!pipeline->pipes)
        return (-1);
    
    i = 0;
    while (i < pipeline->cmd_count - 1)
    {
        pipeline->pipes[i] = malloc(sizeof(int) * 2);
        if (!pipeline->pipes[i])
        {
            // Clean up previously allocated pipes
            while (--i >= 0)
                free(pipeline->pipes[i]);
            free(pipeline->pipes);
            return (-1);
        }
        if (pipe(pipeline->pipes[i]) == -1)
        {
            // Clean up on pipe creation failure
            free(pipeline->pipes[i]);
            while (--i >= 0)
            {
                close(pipeline->pipes[i][0]);
                close(pipeline->pipes[i][1]);
                free(pipeline->pipes[i]);
            }
            free(pipeline->pipes);
            return (-1);
        }
        i++;
    }
    return (0);
}

// Handle heredoc (adapted from your pipex)
int handle_minishell_heredoc(t_cmd *cmd)
{
    int     pipe_fd[2];
    char    *line;
    char    *limiter;
    
    limiter = cmd->infile; // This is how many parsers store heredoc delimiter
    
    if (pipe(pipe_fd) == -1)
        return (-1);
    
    ft_putstr_fd("heredoc> ", 1);
    line = get_next_line(0);
    while (line)
    {
        // Check if line matches limiter (with or without newline)
        if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 && 
            (line[ft_strlen(limiter)] == '\n' || line[ft_strlen(limiter)] == '\0'))
        {
            free(line);
            break;
        }
        ft_putstr_fd(line, pipe_fd[1]);
        free(line);
        ft_putstr_fd("heredoc> ", 1);
        line = get_next_line(0);
    }
    close(pipe_fd[1]);
    return (pipe_fd[0]);
}

// Execute single command (no pipes)
int execute_single_command(t_cmd *cmd, t_env **env)
{
    pid_t   pid;
    char    *path;
    char    **envp;
    int     status;
    int     fd_in = STDIN_FILENO;
    int     fd_out = STDOUT_FILENO;
    
    // Handle input redirection
    if (cmd->heredoc)
    {
        fd_in = handle_minishell_heredoc(cmd);
        if (fd_in < 0)
            return (1);
    }
    else if (cmd->infile)
    {
        fd_in = open(cmd->infile, O_RDONLY);
        if (fd_in < 0)
        {
            perror(cmd->infile);
            return (1);
        }
    }
    
    // Handle output redirection
    if (cmd->outfile)
    {
        if (cmd->append)
            fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0)
        {
            perror(cmd->outfile);
            if (fd_in != STDIN_FILENO)
                close(fd_in);
            return (1);
        }
    }
    
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        if (fd_in != STDIN_FILENO)
            close(fd_in);
        if (fd_out != STDOUT_FILENO)
            close(fd_out);
        return (1);
    }
    
    if (pid == 0) // Child process
    {
        // Redirect file descriptors
        if (fd_in != STDIN_FILENO)
        {
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (fd_out != STDOUT_FILENO)
        {
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        
        // Execute command
        envp = env_to_array(*env);
        if (!envp)
            exit(1);
            
        path = get_path(cmd->args[0], envp);
        if (!path)
        {
            ft_putstr_fd("Command '", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd("' not found\n", 2);
            free_tab(envp);
            exit(127);
        }
        execve(path, cmd->args, envp);
        perror("execve");
        free_tab(envp);
        exit(1);
    }
    else // Parent process
    {
        if (fd_in != STDIN_FILENO)
            close(fd_in);
        if (fd_out != STDOUT_FILENO)
            close(fd_out);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (1);
    }
}

// Execute individual command in pipeline
void execute_pipeline_command(t_pipeline *pipeline, int cmd_index, int fd_in, t_env **env)
{
    t_cmd   *cmd;
    int     fd_out;
    char    *path;
    char    **envp;
    
    cmd = pipeline->cmds[cmd_index];
    
    // Set up input
    if (cmd->heredoc) // Using your teammate's heredoc flag
        fd_in = handle_minishell_heredoc(cmd);
    else if (cmd->infile) // Handle input redirection
    {
        fd_in = open(cmd->infile, O_RDONLY);
        if (fd_in < 0)
        {
            perror(cmd->infile);
            exit(1);
        }
    }
    
    // Set up output
    if (cmd_index == pipeline->cmd_count - 1) // Last command
    {
        if (cmd->outfile)
        {
            if (cmd->append)
                fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0)
            {
                perror(cmd->outfile);
                exit(1);
            }
        }
        else
            fd_out = STDOUT_FILENO;
    }
    else // Not last command
        fd_out = pipeline->pipes[cmd_index][1];
    
    // Redirect file descriptors
    if (fd_in != STDIN_FILENO)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (fd_out != STDOUT_FILENO)
    {
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
    
    // Close all pipe fds in child
    close_all_pipes(pipeline);
    
    // Execute command
    if (is_builtin(cmd))
    {
        exit(handle_builtin(cmd, env));
    }
    else
    {
        envp = env_to_array(*env);
        if (!envp)
            exit(1);
            
        path = get_path(cmd->args[0], envp);
        if (!path)
        {
            ft_putstr_fd("Command '", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd("' not found\n", 2);
            free_tab(envp);
            exit(127);
        }
        execve(path, cmd->args, envp);
        perror("execve");
        free_tab(envp);
        exit(1);
    }
}

int execute_pipeline(t_cmd *cmd_list, t_env **env)
{
    t_pipeline pipeline;
    pid_t *pids;
    int result;
    
    if (!cmd_list)
        return (1);
    
    // Setup pipeline structure and handle single commands
    result = setup_pipeline(cmd_list, &pipeline, env);
    if (result != 999)
        return (result); // Either error (1) or single command result
    
    // Initialize pipeline resources (pipes and pid array)
    pids = init_pipeline_resources(&pipeline);
    if (!pids)
        return (1);
    
    // Execute all commands in the pipeline
    if (execute_all_commands(&pipeline, pids) != 0)
        return (1);
    
    // Close remaining pipes and wait for children
    close_all_pipes(&pipeline);
    result = wait_for_children(&pipeline, pids);
    
    cleanup_pipeline(&pipeline);
    free(pids);
    return (result);
}

void	close_all_pipes(t_pipeline *pipeline)
{
    int	i;
    
    if (!pipeline->pipes)
        return;
    
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

void	cleanup_pipeline(t_pipeline *pipeline)
{
    int	i;
    
    if (pipeline->pipes)
    {
        i = 0;
        while (i < pipeline->cmd_count - 1)
        {
            if (pipeline->pipes[i])
                free(pipeline->pipes[i]);
            i++;
        }
        free(pipeline->pipes);
    }
    if (pipeline->cmds)
        free(pipeline->cmds);
}

int execute_commands(t_cmd *cmds, t_env **env)
{
    return (execute_pipeline(cmds, env));
}