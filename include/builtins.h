

#ifndef BUILTINS_H
# define BUILTINS_H

# include "env.h"
# include "exec.h"

typedef struct s_builtin
{
	char	*name;
	int		(*func)(char **args, t_env **env);
}			t_builtin;

/********** BUILTINS ***********/
int			cd(char **args, t_env **env);
int			cd_dash(t_env **env);
int			echo(char **args);
int			exit_builtin(char **args);
int			export(char **args, t_env **env);
int			env_builtin(t_env *env);
int			pwd(void);
int			unset(char **args, t_env **env);
int			is_builtin(t_cmd *cmd);
int			handle_builtin(t_cmd *cmd, t_env **env);

/********** EXPORT UTILS ***********/
int			set_and_assign(char *arg, t_env **env);
int			print_env_export(t_env *env);
int			is_valid_identifier(const char *str);
void		append_node(t_env **env, t_env *new_node);

/********** UTILS ***********/
int			is_valid_n_flag(const char *arg);
int			is_numeric(const char *str);
int			unset_single_var(t_env **env, char *key);
t_env	**copy_env_to_array(t_env *env, int size);
void	sort_env_by_key(t_env **arr, int size);

#endif
