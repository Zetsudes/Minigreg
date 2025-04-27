
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../greg_lib/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd {
	char **args;
}       t_cmd;

/********* EXEC *********/
char    **split_args(char *line);
char	*find_path(char **dir, char *cmd);
char	*get_path(char *cmd, char **env);
void	execute_command(t_cmd *cmd, char **env);
void	free_tab(char **tab);
char	**copy_env(char **envp);

#endif