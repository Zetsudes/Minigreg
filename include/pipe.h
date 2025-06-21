#ifndef PIPE_H
# define PIPE_H

# include "minishell.h"

typedef struct s_pipeline
{
	t_cmd	**cmds;
	int		cmd_count;
	int		**pipes;
	t_env	**env;
}			t_pipeline;

/********** PIPE ***********/
int			init_pipes(t_pipeline *pipeline);
void		execute_pipeline_command(t_pipeline *pipeline, int cmd_index,
				int fd_in, t_env **env);
int			run_parent_process(pid_t pid, int fd_in, int fd_out);
void		run_child_process(t_cmd *cmd, t_env **env, int fd_in, int fd_out);
void		close_all_pipes(t_pipeline *pipeline);
void		cleanup_pipeline(t_pipeline *pipeline);
int			setup_pipeline(t_cmd *cmd_list, t_pipeline *pipeline, t_env **env);
pid_t		*init_pipeline_resources(t_pipeline *pipeline);
int			execute_commands_pipeline(t_pipeline *pipeline, pid_t *pids);

#endif
