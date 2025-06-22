#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Implementation of the echo builtin command   <3
<3 echo -n = NO newline, echo = newline         <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1; // Assuming there's no -n flag <3
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0; // If we find one then bye bye newline </3
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	// printf("miaou");
		// Test to confirm this function is being executed and not execve <3
	if (newline)
		printf("\n");
	return (0);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Implementation of the env builtin command                  <3
<3 Prints all env var that have a VALUE like this : KEY=VALUE <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	env_builtin(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Implementation of the exit builtin command       <3
<3 Does not exit if there's more than two arguments <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	exit_builtin(char **args)
{
	int	exit_code;

	exit_code = 0;
	write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	if (!is_numeric(args[1]))
	{
		printf("exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	exit_code = atoi(args[1]);
	if (args[2]) // Too many arguments : prints error but doesn't exit <3
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	if (exit_code > 255)
		exit_code = exit_code % 256;
	exit(exit_code); // Exits with the specified code <3
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Implementation of the pwd builtin command      <3
<3 Gets and prints the current working directory  <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s miaou\n", cwd);
			// To make sure it's the one being called and not execve <3
		return (0);
	}
	else
		perror("getcwd()");
	return (1);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Implementation of the unset builtin command  <3
<3 Removes environment variable(s) from env     <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
int	unset(char **args, t_env **env)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		unset_single_var(env, args[i]);
		i++;
	}
	return (0); // Always succeeds, never returns error <3
}
