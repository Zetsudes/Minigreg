
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

char    **split_args(char *line);
char	*get_path(char *cmd, char **envp);
void	execute_command(t_cmd *cmd, char **envp);
void	free_tab(char **tab);

#endif