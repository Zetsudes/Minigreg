/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:47:32 by zamohame          #+#    #+#             */
/*   Updated: 2025/05/02 18:02:26 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
}			t_cmd;

typedef struct s_data
{
    int 	i;
    int 	cmd_count;
    int		num_pipes;
    t_cmd 	*current;
} 			t_data;


char		**split_args(char *line);

/********** PATH ***********/ 
char		*find_path(char **dir, char *cmd);
char		*get_path(char *cmd, char **env);

/********** PIPE ***********/ 
int 	count_commands(t_cmd *cmd);
int		create_pipes(int pipes[][2], int num_pipes);
void    close_pipes(int pipes[][2], int num_pipes);
void	wait_for_children(pid_t pids[], int cmd_count);

/********** EXEC ***********/ 
void		execute_command(t_cmd *cmd, char **env);
void		execute_external_command(t_cmd *cmd, char **envp);

/********** EXEX UTILS ***********/ 
void		free_tab(char **tab);
void		init_data(t_data *data, t_cmd *cmd);
void		run_child_process(t_data *data, int pipes[][2], char **envp);

void		print_lava_intro(void);

#endif
