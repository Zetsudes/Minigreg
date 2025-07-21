/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:12:52 by marvin            #+#    #+#             */
/*   Updated: 2025/07/21 08:12:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Implementation of the export builtin command.
** Exports variable(s), validates args, and assigns.
** If no arguments, prints all variables in alphabetical order.
*/
int	export(char **args, t_env **env)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
		return (print_env_export(*env));
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else if (ft_strchr(args[i], '='))
			set_and_assign(args[i], env);
		else
			set_env_value(env, args[i], NULL);
		i++;
	}
	return (status);
}

/*
** Updates existing variable or adds new one.
** Helper function used in export().
*/
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

/*
** Prints all env variables in alphabetical order.
** Format: declare -x KEY="VALUE"
** Called when export has no arguments.
*/
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

/*
** Checks if a variable name is valid for export.
*/
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
** Sorts an array of environment variables by key (bubble sort).
*/
void	sort_env_by_key(t_env **arr, int size)
{
	int		i;
	int		sorted;
	t_env	*tmp;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		i = 0;
		while (i < size - 1)
		{
			if (ft_strcmp(arr[i]->key, arr[i + 1]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				sorted = 0;
			}
			i++;
		}
	}
}
