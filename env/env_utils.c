
#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Frees all memory used by the environment linked list <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void	free_env_list(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	*handle_alloc_failure(char *key, char *value, t_env *first)
{
	free(key);
	free(value);
	free_env_list(first);
	return (NULL);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Creates a new env node and adds it to the beginning of the list  <3
<3 Helper function used in set_env_value() in handle_env.c file     <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	add_new_env_node(t_env **env, char *key, char *value)
{
	t_env	*new;

	new = create_node(key, value);
	if (!new)
		return (0); // no need to free 'new' or call free_env_list
	new->next = *env;
	*env = new;
	return (1);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Function to join three strings together        <3
<3 Used to create "KEY=VALUE" from separate parts <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1); // Null terminator <3
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1); // Copies each part into result <3
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Counts number of nodes in env list <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	env_list_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

