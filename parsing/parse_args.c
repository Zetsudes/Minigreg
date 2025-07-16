#include "../include/minishell.h"

char	*process_token(char *tok, t_env *env)
{
	char	*out;

	if (!tok)
		return (ft_strdup(""));
	out = expand_var(tok, env);
	if (!out)
		out = ft_strdup("");
	return (out);
}


char	**copy_args(char **tokens, int *i, t_env *env)
{
	char	**args;
	int		start;
	int		j;

	start = *i;
	while (tokens[*i] && ft_strcmp(tokens[*i], "|")
		&& ft_strcmp(tokens[*i], ";")
		&& !is_token_operator(tokens[*i]))
		(*i)++;
	args = malloc(sizeof(char *) * (*i - start + 1));
	if (!args)
		return (NULL);
	j = 0;
	while (start < *i)
	{
		args[j] = process_token(tokens[start], env);
		if (!args[j])
		{
			while (j-- > 0)
				free(args[j]);
			free(args);
			return (NULL);
		}
		j++;
		start++;
	}
	args[j] = NULL;
	return (args);
}
