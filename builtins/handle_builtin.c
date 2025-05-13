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

void	handle_builtin(t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "pwd", 3) == 0 && ft_strlen(cmd->cmd) == 3)
		pwd();
	else if (ft_strncmp(cmd->cmd, "cd", 2) == 0 && ft_strlen(cmd->cmd) == 2)
		cd(cmd->args);
	else if (ft_strncmp(cmd->cmd, "cd -", 4) == 0 && ft_strlen(cmd->cmd) == 4)
		pd();
}

int is_builtin(t_cmd *cmd)
{
	return (ft_strncmp(cmd->cmd, "pwd", 3) == 0 || ft_strncmp(cmd->cmd, "cd", 2) == 0 || ft_strncmp(cmd->cmd, "cd -", 4) == 0);
}

