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

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Function that handles all builtin commands       <3
<3 Called when a command is identified as a builtin <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
void	handle_builtin(t_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0 && (!cmd->args[1] || ft_strncmp(cmd->args[1], "-", 2) != 0))
		cd(cmd->args, env);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0 && cmd->args[1] && ft_strncmp(cmd->args[1], "-", 2) == 0)
		cd_dash(env);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		export(cmd->args, env);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		unset(cmd->args, env);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		env_builtin(*env);
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		echo(cmd->args);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		exit_builtin(cmd->args);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Checks if a command is a builtin command         <3
<3 Called when a command is identified as a builtin <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	is_builtin(t_cmd *cmd)
{
	static const char	*builtins[] = {
		"pwd", "cd", "export", "unset", "env", "exit", "echo", NULL
	};
	int					i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(cmd->args[0], builtins[i], ft_strlen(builtins[i]) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}


