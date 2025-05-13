

#include "../include/minishell.h"

char    *get_env_value(t_env *env, char *key)
{

    while (env)
    {
        if (ft_strncmp(key, env->key, ft_strlen(key)) == 0 && ft_strlen(key) == ft_strlen(env->key))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}