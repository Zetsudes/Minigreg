/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:47:32 by zamohame          #+#    #+#             */
/*   Updated: 2025/06/22 19:26:08 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "env.h"
# include "pipe.h"
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_cmd
{
	char **args;        // arguments (ex: ["echo", "hello"])
	char *infile;       // fichier d'entrée (<)
	char *outfile;      // fichier de sortie (>)
	int append;         // 1 si >>
	int heredoc;        // 1 si <<
	struct s_cmd *next; // pour les pipes
}		t_cmd;

/********** PARSING ***********/
char	**tokenize_line(char *line);
t_cmd	*parse_tokens(char **tokens, t_env *env);
void	free_cmd_list(t_cmd *cmd_list);
char	*process_token(char *token, t_env *env);

/********** EXPAND ***********/
char	*expand_var(const char *arg, t_env *env);

/********** PARSING UTILS ***********/
char	**copy_args(char **tokens, int *i, t_env *env);
int		count_args(char **tokens, int i);
int		is_token_operator(const char *token);
int		set_redirection(t_cmd *cmd, char *op, char *file);
int		ft_strcmp(const char *s1, const char *s2);
t_cmd	*init_cmd(void);

/********** PATH ***********/
char	*find_path(char **dir, char *cmd);
char	*get_path(char *cmd, char **env);

/********** EXEC ***********/
int		execute_single_command(t_cmd *cmd, t_env **env);
int		execute_commands(t_cmd *cmd_list, t_env **env);
void	exec_command(t_cmd *cmd, t_env **env);
int		wait_for_children(t_pipeline *pipeline, pid_t *pids);
int	execute_builtin(t_cmd *cmd, t_env **env, int fd_in, int fd_out);

/********** REDIRECTIONS ***********/
int		handle_heredoc(t_cmd *cmd);
int		handle_input_redirection(t_cmd *cmd, int *fd_in);
int		handle_output_redirection(t_cmd *cmd, int fd_in, int *fd_out);

/********** EXEC UTILS ***********/
void	free_tab(char **tab);
t_cmd	**cmd_list_to_array(t_cmd *cmd_list, int *count);
int		handle_fork_error(pid_t *pids, int i, t_pipeline *pipeline);
void	update_fds(int *fd_in, t_pipeline *pipeline, int i);

#endif
