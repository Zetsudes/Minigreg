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

int	cd(char **args)
{
	char	*home;

	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			perror("getenv()");
			return (1);
		}
		if (chdir(home) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	else if (chdir(args[1]) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	pd(void)
{
	char	*old_pwd;

	old_pwd = getenv("OLDPWD");
	if (!old_pwd)
	{
		perror("getenv()");
		return (1);
	}
	if (chdir(old_pwd) == -1)
	{
		perror("cd -");
		return (1);
	}
	ft_printf("%s miaou\n", old_pwd);
	return (0);
}
