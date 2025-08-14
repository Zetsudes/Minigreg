/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:07:29 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 09:26:21 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Implementation of the echo builtin command.
** echo -n = no newline; echo = newline.
*/
int	echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/*
** Implementation of the env builtin command.
** Prints all env vars that have a value as KEY=VALUE.
*/
int	env_builtin(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value && ft_strcmp(tmp->key, "?") != 0)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

/*
** Implementation of the exit builtin command.
** Does not exit if there are more than two arguments.
*/
int	exit_builtin(char **args)
{
	int	exit_code;

	exit_code = 0;
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	if (!is_numeric(args[1]))
	{
		print_error_message(args, 1, "exit: ", ": numeric argument required");
		exit(2);
	}
	exit_code = atoi(args[1]);
	if (args[2])
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	if (exit_code > 255)
		exit_code = exit_code % 256;
	exit(exit_code);
}

/*
** Implementation of the pwd builtin command.
** Gets and prints the current working directory.
*/
int	pwd(char **av, t_env **env)
{
	char	buf[PATH_MAX];
	char	*pwd;

	(void)av;
	if (getcwd(buf, sizeof(buf)))
		ft_putendl_fd(buf, STDOUT_FILENO);
	else
	{
		pwd = get_env_value(*env, "PWD");
		if (pwd)
			ft_putendl_fd(pwd, STDOUT_FILENO);
		else
		{
			perror("pwd");
			return (1);
		}
	}
	return (0);
}

/*
** Implementation of the unset builtin command.
** Removes environment variable(s) from env.
*/
int	unset(char **args, t_env **env)
{
	int	i;
	int	status;

	if (!args[1])
		return (0);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]) || args[i][0] == '\0')
		{
			print_error_message(args, i, "unset: `",
				"': not a valid identifier");
			status = 1;
		}
		else
			unset_single_var(env, args[i]);
		i++;
	}
	return (status);
}
