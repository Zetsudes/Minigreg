/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:23:51 by zamohame          #+#    #+#             */
/*   Updated: 2025/08/14 10:49:18 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_getcwd_warning(void)
{
	ft_putendl_fd("cd: error retrieving current directory: getcwd: "
		"cannot access parent directories: No such file or directory",
		STDERR_FILENO);
}

char	*pop_last(const char *oldpwd)
{
	char	*dup;
	char	*slash;

	dup = ft_strdup((char *)oldpwd);
	if (!dup)
		return (NULL);
	slash = ft_strrchr(dup, '/');
	if (slash && slash != dup)
		*slash = '\0';
	else
	{
		free(dup);
		return (ft_strdup("/"));
	}
	return (dup);
}

static int	update_pwd(t_env **env, char *oldpwd, const char *arg)
{
	char	cwd[PATH_MAX];
	char	*newpwd;

	if (getcwd(cwd, sizeof(cwd)))
		newpwd = ft_strdup(cwd);
	else
	{
		print_getcwd_warning();
		newpwd = get_fallback_pwd(arg, oldpwd);
	}
	if (!newpwd)
	{
		free(oldpwd);
		return (1);
	}
	if (oldpwd)
		set_env_value(env, "OLDPWD", oldpwd);
	else
		set_env_value(env, "OLDPWD", "");
	set_env_value(env, "PWD", newpwd);
	free(newpwd);
	free(oldpwd);
	return (0);
}

int	cd(char **av, t_env **env)
{
	char	*target;
	char	*oldpwd;
	char	*pwd_env;

	if (av[1] && av[2])
	{
		ft_putendl_fd("cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	target = get_cd_target(av, env);
	if (!target)
		return (1);
	pwd_env = get_env_value(*env, "PWD");
	if (pwd_env)
		oldpwd = ft_strdup((char *)pwd_env);
	else
		oldpwd = NULL;
	if (chdir(target) == -1)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	return (update_pwd(env, oldpwd, target));
}

int	cd_dash(t_env **env)
{
	char	*oldpwd;
	char	*dup;

	oldpwd = get_env_value(*env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
		return (1);
	}
	if (chdir(oldpwd) == -1)
	{
		perror("cd");
		return (1);
	}
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	dup = ft_strdup((char *)oldpwd);
	if (!dup)
		return (1);
	return (update_pwd(env, dup, oldpwd));
}
