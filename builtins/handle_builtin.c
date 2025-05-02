/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:06:09 by zamohame          #+#    #+#             */
/*   Updated: 2025/05/02 18:02:23 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "pwd", 3))
		pwd();
	else if (ft_strncmp(cmd, "cd", 2))
		cd(cmd);
}
