#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Adds a new environment variable node to the end of the linked list <3
<3 Helper function used in set_and_assign() in export.c file          <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/ 
void	append_node(t_env **env, t_env *new_node)
{
	t_env   *tmp;

	if (!*env)
		*env = new_node; // If list is empty new_node becomes the head <3
	else
	{
		tmp = *env;
		while (tmp->next) // Finds the last node <3
			tmp = tmp->next;
		tmp->next = new_node; // Adds new_node to the end <3
	}
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Checks if a string is a valid -n flag for the echo command <3
<3 Helper function used in echo() in builtins.c file          <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int is_valid_n_flag(const char *arg) 
{
    int i;

    i = 0;
    if (!arg || arg[0] != '-' || arg[1] != 'n')
        return 0;

    i = 2;
    while (arg[i]) 
    {
        if (arg[i] != 'n') // After '-n' only 'n' characters are allowed <3
            return 0;
        i++;
    }
    return (1);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Checks if a string is a positive or negative integer       <3
<3 Helper function used in exit_builtin() in builtins.c file  <3
<3 In bash exit code changes if there's a non numeric char    <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int is_numeric(const char *str)
{
    int i;

    i = 0;
    if (!str || str[0] == '\0')
        return 0;
    if (str[i] == '+' || str[i] == '-') // If there's ONE sign bash skips it. If there's more than ONE it's game over </3
        i++;
    if (str[i] == '\0')
        return 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i])) // All remaining chars must be digits <3
            return 0;
        i++;
    }
    return (1);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Removes an environment variable from the env list using its key  <3
<3 Helper function used in unset() in builtins.c file               <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	unset_single_var(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	while (curr)
	{
		if (ft_strncmp(key, curr->key, ft_strlen(key) + 1) == 0) // Checks if curr's key matches exactly with the key to unset (+ 1 for '\0') <3
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

t_env	**copy_env_to_array(t_env *env, int size)
{
	t_env	**arr;
	int		i = 0;

	arr = malloc(sizeof(t_env *) * (size + 1));
	if (!arr)
		return (NULL);
	while (env && i < size)
	{
		arr[i++] = env;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

