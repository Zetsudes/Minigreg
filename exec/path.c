/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <marvin@student.42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:12:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/13 13:12:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Searches through PATH if command exists  <3
<3 Helper function used in get_path() below <3 
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	*find_path(char **dir, char *cmd)
{
	int		i;
	char	*tmp;
	char	*final_path;

	i = 0;
	while (dir[i])
	{
		tmp = ft_strjoin(dir[i], "/");
		final_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(final_path, F_OK | X_OK) == 0)
			return (final_path);
		free(final_path);
		i++;
	}
	return (NULL);
}

/*
<3 Checks if command contains '/' or starts with '.' <3
<3 Returns a copy of cmd if valid, NULL otherwise    <3
*/
static char	*check_direct_cmd(char *cmd)
{
	DIR	*dir;

	if (ft_strchr(cmd, '/') || cmd[0] == '.')
	{
		if (access(cmd, F_OK) == 0)
		{
			dir = opendir(cmd);
			if (dir)
			{
				closedir(dir);
				return (ft_strdup(cmd));
			}
			return (ft_strdup(cmd));
		}
		return (NULL);
	}
	return (NULL);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Finds the full path to a command <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*path;
	char	**dir;
	char	*final_path;

	final_path = check_direct_cmd(cmd);
	if (final_path)
		return (final_path);
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	path = env[i] + 5;
	dir = ft_split(path, ':');
	final_path = find_path(dir, cmd);
	free_tab(dir);
	return (final_path);
}
