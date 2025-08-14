/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 08:17:34 by zamohame          #+#    #+#             */
/*   Updated: 2025/08/14 09:27:26 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_cd_target(char **av, t_env **env)
{
	char	*target;

	target = av[1];
	if (!target || !*target)
	{
		target = get_env_value(*env, "HOME");
		if (!target)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
		if (!*target)
			return (NULL);
	}
	return (target);
}

char	*get_fallback_pwd(const char *arg, char *oldpwd)
{
	if (ft_strcmp(arg, "..") == 0)
		return (pop_last(oldpwd));
	return (ft_strdup((char *)oldpwd));
}
