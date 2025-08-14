/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 21:00:02 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 10:01:28 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
int			cd(char **av, t_env **env);
int			cd_dash(t_env **env);
int			echo(char **args);
int			exit_builtin(char **args);
int			export(char **args, t_env **env);
int			env_builtin(t_env *env);
int			pwd(char **av, t_env **env);
int			unset(char **args, t_env **env);
int			is_builtin(t_cmd *cmd);
int			handle_builtin(t_cmd *cmd, t_env **env);

/********** CD UTILS ***********/
char		*get_fallback_pwd(const char *arg, char *oldpwd);
char		*get_cd_target(char **av, t_env **env);
char		*pop_last(const char *oldpwd);

/********** EXPORT UTILS ***********/
int			set_and_assign(char *arg, t_env **env);
int			print_env_export(t_env *env);
int			is_valid_identifier(const char *str);
void		append_node(t_env **env, t_env *new_node);
char		*expand_export_key(const char *key, t_env *env);
int			export_with_equal(char *arg, t_env **env, t_env *current_env);
int			handle_export_arg(char **args, int i, t_env **env);

/********** UTILS ***********/
int			is_valid_n_flag(const char *arg);
int			is_numeric(const char *str);
int			unset_single_var(t_env **env, char *key);
t_env		**copy_env_to_array(t_env *env, int size);
void		sort_env_by_key(t_env **arr, int size);
void		print_error_message(char **args, int i, char *prefix, char *suffix);
void		print_error_message_str(char *str, char *prefix, char *suffix);

#endif
