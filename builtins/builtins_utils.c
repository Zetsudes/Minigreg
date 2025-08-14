/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:07:25 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 09:26:14 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Adds a new environment variable node to the end of the linked list.
** Helper function used in set_and_assign() in export.c.
*/
void	append_node(t_env **env, t_env *new_node)
{
	t_env	*tmp;

	if (!*env)
		*env = new_node;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

/*
** Checks if a string is a valid -n flag for the echo command.
** Helper function used in echo() in builtins.c.
*/
int	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/*
** Checks if a string is a positive or negative integer.
** Helper function used in exit_builtin() in builtins.c.
*/
int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** Removes an environment variable from the env list using its key.
** Helper function used in unset() in builtins.c.
*/
int	unset_single_var(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	while (curr)
	{
		if (ft_strncmp(key, curr->key, ft_strlen(key) + 1) == 0)
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

/*
** Copies the environment list to an array of pointers.
*/
t_env	**copy_env_to_array(t_env *env, int size)
{
	t_env	**arr;
	int		i;

	i = 0;
	arr = malloc(sizeof(t_env *) * (size + 1));
	if (!arr)
		return (NULL);
	while (env && i < size)
	{
		arr[i] = env;
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
