
#ifndef ENV_H
# define ENV_H

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

/********** CREATE ENV LIST ***********/
t_env				*init_list(char **envp);
t_env				*create_node(char *key, char *value);
t_env				*handle_var(char *env_str, t_env *first);
char				*get_split_value(char **split, t_env *first);
void				add_node(t_env **first, t_env **last, t_env *new_node);

/********** HANDLE ENV ***********/
char				*get_env_value(t_env *env, char *key);
char				**env_to_array(t_env *env);
int					set_env_value(t_env **env, char *key, char *value);
int					update_if_key(t_env *env, t_env *new_node);
int					set_and_assign(char *arg, t_env **env);
void				append_node(t_env **env, t_env *new_node);

/********** ENV UTILS ***********/
void				free_env_list(t_env *head);
char				*ft_strjoin_3(const char *s1, const char *s2,
						const char *s3);
int					add_new_env_node(t_env **env, char *key, char *value);
int					env_list_size(t_env *env);
int					fill_env_array(char **arr, t_env *env);

#endif
