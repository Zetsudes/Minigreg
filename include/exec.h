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
# include "parsing.h"   /* pour t_cmd */
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>

/* ---------- EXECUTION API ---------- */

int		execute_single_command(t_cmd *cmd, t_env **env);
int		execute_commands(t_cmd *cmd_list, t_env **env);
void	exec_command(t_cmd *cmd, t_env **env);
int		wait_for_children(t_pipeline *pipeline, pid_t *pids);
int		execute_builtin(t_cmd *cmd, t_env **env, int fd_in, int fd_out);
int		execute_command_sequence(t_cmd *cmd_list, t_env **env);

/* ---------- path ---------- */
char    *get_path(char *cmd, char **envp);
char    *find_path(char **dir, char *cmd);

/* ---------- redirections & utilitaires ---------- */

int		handle_heredoc(t_cmd *cmd);
int		handle_input_redirection(t_cmd *cmd, int *fd_in);
int		handle_output_redirection(t_cmd *cmd, int fd_in, int *fd_out);

void	free_tab(char **tab);
t_cmd	**cmd_list_to_array(t_cmd *cmd_list, int *count);
int		handle_fork_error(pid_t *pids, int i, t_pipeline *pipeline);
void	update_fds(int *fd_in, t_pipeline *pipeline, int i);

#endif