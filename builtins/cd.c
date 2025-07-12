/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:23:51 by zamohame          #+#    #+#             */
/*   Updated: 2025/04/29 18:27:27 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Implementation of the cd builtin command               <3
<3 Changes the current directory, updates PWD and OLDPWD  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	cd(char **args, t_env **env)
{
	char	cwd[PATH_MAX];
	char	*old_pwd;
	char	*target;

	old_pwd = getcwd(NULL, 0); // Saves current directory before changing <3
	if (!old_pwd)
		return (perror("getcwd"), 1);
	if (args[1] && args[2])
	{
		printf("cd: too many arguments\n");
		free(old_pwd);
		return (1);
	}
	set_env_value(env, "OLDPWD", old_pwd); // Updates OLDPWD with current directory <3
	if (!args[1])
	{
		target = get_env_value(*env, "HOME"); // No argument -> go to HOME <3
		if (!target)
			return (perror("cd"), free(old_pwd), 1);
	}
	else
		target = args[1];
	if (chdir(target) == -1) // Changes directory <3
		return (perror("cd"), free(old_pwd), 1);
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("getcwd"), free(old_pwd), 1);
	set_env_value(env, "PWD", cwd); // Updates PWD with new directory <3
	free(old_pwd);
	return (0);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Implementation of the cd - builtin command <3
<3 Uses OLDPWD to know where to go back to    <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	cd_dash(t_env **env)
{
	char	*oldpwd = get_env_value(*env, "OLDPWD");
	char	cwd[PATH_MAX];

	if (!oldpwd || chdir(oldpwd) == -1) // Changes to previous directory <3
		return (perror("cd -"), 1);
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("getcwd"), 1);
	set_env_value(env, "PWD", cwd); // Updates PWD <3
	ft_printf("%s\n", cwd); // Prints new directory <3
	return (0);
}

