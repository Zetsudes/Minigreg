
#include "../include/minishell.h"

void	print_error_message(char **args, int i, char *prefix, char *suffix) // Array <3
{
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(args[i], 2);
	ft_putendl_fd(suffix, 2);
}

void	print_error_message_str(char *str, char *prefix, char *suffix) // String <3
{
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd(suffix, 2);
}

char	*expand_export_key(const char *key, t_env *env)
{
	if (key[0] != '$')
		return (ft_strdup((char *)key));
	key++;
	if (!*key)
		return (NULL);
	return (ft_strdup(get_env_value(env, (char *)key)));
}

int	handle_export_arg(char **args, int i, t_env **env)
{
	if (ft_strchr(args[i], '='))
	{
		if (export_with_equal(args[i], env, *env))
			return (1);
		return (0);
	}
	if (!is_valid_identifier(args[i]))
	{
		print_error_message(args, i, "export: `", "': not a valid identifier");
		return (1);
	}
	set_env_value(env, args[i], NULL);
	return (0);
}

int	export_with_equal(char *arg, t_env **env, t_env *current_env)
{
	char	*eq;
	char	*raw_key;
	char	*key;
	char	*joined;

	eq = ft_strchr(arg, '=');
	raw_key = ft_substr(arg, 0, eq - arg);
	if (!raw_key)
		return (1);
	key = expand_export_key(raw_key, current_env);
	free(raw_key);
	if (!key || !is_valid_identifier(key))
	{
		 print_error_message_str(arg, "export: `", "': not a valid identifier");
		free(key);
		return (1);
	}
	joined = ft_strjoin(key, eq);
	set_and_assign(joined, env);
	free(joined);
	free(key);
	return (0);
}