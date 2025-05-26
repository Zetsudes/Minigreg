

#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Searches for env var by key and returns its value            <3
<3 Used to get values like HOME, PATH, PWD, ...                 <3 
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char    *get_env_value(t_env *env, char *key)
{

    while (env)
    {
        if (ft_strncmp(key, env->key, ft_strlen(env->key) + 1) == 0) // Checks if keys match <3
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Sets/updates env var with  given key and value             <3
<3 If var exists updates its value. If not, creates a new one <3 
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(key, tmp->key, ft_strlen(tmp->key) + 1) == 0) // Tries to find existing key <3
		{
			free(tmp->value); // Found -> updates its value <3
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = ft_strdup(""); // Empty string if NULL value <3
			if (!tmp->value)
				return (0);
			return (1);
		}
		tmp = tmp->next;
	}
	return (add_new_env_node(env, key, value)); // Key not found -> create new one <3
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Converts env list to an array of strings <3
<3 "KEY=VALUE" format                       <3 
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	**env_to_array(t_env *env)
{
	char	**arr;

	arr = malloc(sizeof(char *) * (env_list_size(env) + 1));
	if (!arr)
		return (NULL);
	if (!fill_env_array(arr, env))
	{
		free(arr);
		return (NULL);
	}
	return (arr);
}


