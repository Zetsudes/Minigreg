
#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Creates new env var node with given key and value  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
t_env	*create_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key); // Duplicates the key <3
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	if (value)
		node->value = ft_strdup(value); // Duplicates the value <3
	else
		node->value = ft_strdup(""); // If value is NULL <3
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}
/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Gets the value from the split result of 'VAR=value'  <3
<3 Helper function used in handle_var() below           <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	*get_split_value(char **split, t_env *first)
{
	if (!split)
	{
		free_env_list(first);
		return (NULL);
	}
	if (!split[0] || split[0][0] == '\0') // No key name </3
	{
		free_tab(split);
		return (NULL);
	}
	if (split[1] == NULL) // No value (VAR=) </3
		return ("");
	return (split[1]);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Parses env string ("VAR=value") and creates env node <3
<3 Used to process env var and create a node            <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
t_env	*handle_var(char *env_str, t_env *first)
{
	t_env	*new_node;
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return (create_node(env_str, ""));
	key = ft_substr(env_str, 0, eq - env_str);
	value = ft_strdup(eq + 1);
	if (!key || !value)
		return (handle_alloc_failure(key, value, first));
	new_node = create_node(key, value);
	free(key);
	free(value);
	if (!new_node)
		return (handle_alloc_failure(NULL, NULL, first));
	return (new_node);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Adds new node to env linked list                         <3
<3 Keeps track of first and last nodes for better appending <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void	add_node(t_env **first, t_env **last, t_env *new_node)
{
	if (!*first)
	{
		*first = new_node;
		*last = new_node;
	}
	else
	{
		(*last)->next = new_node;
		*last = new_node;
	}
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Initializes env linked list from envp array                          <3
<3 Converts the array of strings "PATH=/usr/bin" into a linked list     <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
t_env	*init_list(char **envp)
{
	int i;
	t_env *first;
	t_env *last;
	t_env *new_node;

	if (!envp || !envp[0])
		return (init_minimal_env());
	first = NULL;
	last = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = handle_var(envp[i], first); // Parses and creates node <3
		if (!new_node)
			return (NULL);
		add_node(&first, &last, new_node); // Add to list <3
		i++;
	}
	return (first);
}
