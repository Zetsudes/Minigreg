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
		if (handle_export_arg(args, i, env))
			status = 1;
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
