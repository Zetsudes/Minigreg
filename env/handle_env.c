/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <marvin@student.42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:40:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/13 12:40:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Searches for env var by key and returns its value            <3
<3 Used to get values like HOME, PATH, PWD, ...                 <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(key, env->key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Sets/updates env var with given key and value               <3
<3 If var exists updates its value. If not, creates a new one  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(key, tmp->key) == 0)
		{
			free(tmp->value);
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = ft_strdup("");
			if (!tmp->value)
				return (0);
			return (1);
		}
		tmp = tmp->next;
	}
	return (add_new_env_node(env, key, value));
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Converts env list to an array of strings <3
<3 "KEY=VALUE" format                       <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
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

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Initializes env list when no envp is provided              <3
<3 Creates a minimal env with PWD, PATH, HOME, SHLVL          <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
t_env	*init_minimal_env(void)
{
	t_env	*first;
	t_env	*last;
	char	cwd[PATH_MAX];
	t_env	*node;

	first = NULL;
	last = NULL;
	if (getcwd(cwd, sizeof(cwd)))
	{
		node = create_node("PWD", cwd);
		if (node)
			add_node(&first, &last, node);
	}
	node = create_node("PATH", "/usr/bin:/bin:/usr/local/bin");
	if (node)
		add_node(&first, &last, node);
	node = create_node("HOME", "/home/yourname");
	if (node)
		add_node(&first, &last, node);
	node = create_node("SHLVL", "1");
	if (node)
		add_node(&first, &last, node);
	return (first);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Fills array with "KEY=VALUE"                                 <3
<3 Converts list to array                                       <3
<3 Helper function used in env_to_array() in handle_env.c file  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	fill_env_array(char **arr, t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		arr[i] = ft_strjoin_3(env->key, "=", env->value);
		if (!arr[i])
		{
			while (i > 0)
				free(arr[--i]);
			return (0);
		}
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	return (1);
}
