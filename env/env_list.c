/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/14 09:28:07 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*create_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = ft_strdup("");
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

char	*get_split_value(char **split, t_env *first)
{
	if (!split)
	{
		free_env_list(first);
		return (NULL);
	}
	if (!split[0] || split[0][0] == '\0')
	{
		free_tab(split);
		return (NULL);
	}
	if (split[1] == NULL)
		return ("");
	return (split[1]);
}

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

t_env	*init_list(char **envp)
{
	int		i;
	t_env	*first;
	t_env	*last;
	t_env	*new_node;

	if (!envp || !envp[0])
		return (init_minimal_env());
	first = NULL;
	last = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = handle_var(envp[i], first);
		if (!new_node)
			return (NULL);
		add_node(&first, &last, new_node);
		i++;
	}
	return (first);
}
