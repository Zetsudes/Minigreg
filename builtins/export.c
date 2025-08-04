/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:23:51 by zamohame          #+#    #+#             */
/*   Updated: 2025/08/05 00:00:00 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_export_key(const char *key, t_env *env)
{
	if (key[0] != '$')
		return (ft_strdup((char *)key));
	key++;
	if (!*key)
		return (NULL);
	return (ft_strdup(get_env_value(env, (char *)key)));
}

static int	export_with_equal(char *arg, t_env **env, t_env *current_env)
{
	char	*eq;
	char	*raw_key;
	char	*key;
	char	*joined;

	eq = ft_strchr(arg, '=');
	raw_key = ft_substr(arg, 0, eq - arg);
	if (!raw_key)
		return (1);
	key = expand_export_key(raw_key, current_env);
	free(raw_key);
	if (!key || !is_valid_identifier(key))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		free(key);
		return (1);
	}
	joined = ft_strjoin(key, eq);
	set_and_assign(joined, env);
	free(joined);
	free(key);
	return (0);
}

int	export(char **args, t_env **env)
{
	int		i;
	int		status;
	i = 1;
	status = 0;
	
	if (!args[1])
		return (print_env_export(*env));
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			if (export_with_equal(args[i], env, *env))
				status = 1;
			i++;
			continue ;
		}
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
			set_env_value(env, args[i], NULL);
		i++;
	}
	return (status);
}

int	set_and_assign(char *arg, t_env **env)
{
	t_env	*new_node;

	new_node = handle_var(arg, *env);
	if (!new_node)
		return (1);
	if (!set_env_value(env, new_node->key, new_node->value))
		return (1);
	free(new_node->key);
	free(new_node->value);
	free(new_node);
	return (0);
}

int	print_env_export(t_env *env)
{
	t_env	**arr;
	int		i;
	int		size;

	size = env_list_size(env);
	arr = copy_env_to_array(env, size);
	if (!arr)
		return (1);
	sort_env_by_key(arr, size);
	i = 0;
	while (i < size)
	{
		printf("declare -x %s", arr[i]->key);
		if (arr[i]->value)
			printf("=\"%s\"", arr[i]->value);
		printf("\n");
		i++;
	}
	free(arr);
	return (0);
}

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	sort_env_by_key(t_env **arr, int size)
{
	int		swapped;
	int		i;
	t_env	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < size - 1)
		{
			if (ft_strcmp(arr[i]->key, arr[i + 1]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}
