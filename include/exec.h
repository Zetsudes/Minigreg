/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:47:32 by zamohame          #+#    #+#             */
/*   Updated: 2025/08/14 09:56:05 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "env.h"
# include "parsing.h" /* pour t_cmd */
# include "pipe.h"
# include <dirent.h>
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

/* ---------- EXECUTION API ---------- */
int		execute_single_command(t_cmd *cmd, t_env **env);
int		execute_commands(t_cmd *cmd_list, t_env **env);
void	exec_command(t_cmd *cmd, t_env **env);
int		wait_for_children(t_pipeline *pipeline, pid_t *pids);
int		execute_builtin(t_cmd *cmd, t_env **env, int fd_in, int fd_out);
int		execute_command_sequence(t_cmd *cmd_list, t_env **env);

/* ---------- COMMAND UTILS ---------- */
void	validate_path(char *path, char **envp);
int		setup_and_validate_command(t_cmd *cmd, int *fd_in, int *fd_out);
int		process_command_group(t_cmd *cmd_list, t_env **env);
void	save_fds(int fd_in, int fd_out, int saved[2]);

/* ---------- ERRORS ---------- */
void	handle_execve_error(t_cmd *cmd, char **envp);
int		handle_fork_error(pid_t *pids, int i, t_pipeline *pipeline);

/* ---------- PATH ---------- */
char	*get_path(char *cmd, char **envp);
char	*find_path(char **dir, char *cmd);

/* ---------- REDIRECTIONS & UTILITAIRES ---------- */
int		handle_heredoc(t_cmd *cmd);
int		handle_input_redirection(t_cmd *cmd, int *fd_in);
int		handle_output_redirection(t_cmd *cmd, int fd_in, int *fd_out);
int		process_heredoc_line(char *line, char *limiter, int write_fd);

void	free_tab(char **tab);
void	cleanup_and_exit(char **envp, char *path, int exit_code);
t_cmd	**cmd_list_to_array(t_cmd *cmd_list, int *count);
void	update_fds(int *fd_in, t_pipeline *pipeline, int i);

/* ---------- MAIN UTILS ---------- */
char	*expand_buffer(char *line, size_t *cap, size_t *len, char c);
char	*read_line_noninteractive(void);
void	process_command_line(char *line, t_env **env);
char	*get_input_line(const char *prompt);

#endif
