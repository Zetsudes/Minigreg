#include "../include/minishell.h"

static int	token_is_quoted(const char *s)
{
	while (*s)
	{
		if (*s == '\'' || *s == '"')
		{
			char q = *s++;
			const char *start = s;
			while (*s && *s != q)
				s++;
			if (*s == q && s - start > 0)
				return (1);
		}
		else
			s++;
	}
	return (0);
}

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

static int      count_effective_args(char **tk, int j, t_env *env)
{
	int count = 0;
	while (tk[j] && ft_strcmp(tk[j], "|") && ft_strcmp(tk[j], ";"))
	{
		if (is_token_operator(tk[j]))
		{
			j += 2;
			continue;
		}
		char *tmp = process_token(tk[j], env);
		if (tmp && (tmp[0] != '\0'
				|| token_is_quoted(tk[j])
				|| is_empty_quotes(tk[j])))
				count++;
		free(tmp);
		j++;
	}
	return (count);
}

char    **copy_args(char **tokens, int *i, t_cmd *cmd, t_env *env)
{
	int     j;
	int     count;
	char    **args;
	int     k;

	j = *i;
	count = count_effective_args(tokens, j, env);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	k = 0;
	while (tokens[j] && ft_strcmp(tokens[j], "|") && ft_strcmp(tokens[j], ";"))
	{
		if (is_token_operator(tokens[j]))
		{
			char *op = tokens[j];
			char *file = tokens[j + 1];
			if (!file || is_token_operator(file))
			{
				print_syntax_error(file);
				while (k-- > 0)
					free(args[k]);
				free(args);
				return (NULL);
			}
			char *fname = process_token(file, env);
			if (!fname || !set_redirection(cmd, op, fname))
			{
				while (k-- > 0)
					free(args[k]);
				free(args);
				free(fname);
				return (NULL);
			}
			free(fname);
			j += 2;
			if (cmd->redir_error)
			{
				while (tokens[j] && ft_strcmp(tokens[j], "|") && ft_strcmp(tokens[j], ";"))
					j++;
				break;
			}
			continue;
		}
		char *tmp = process_token(tokens[j], env);
		if (tmp && (tmp[0] != '\0'
				|| token_is_quoted(tokens[j])
				|| is_empty_quotes(tokens[j])))
				args[k++] = tmp;
		else
			free(tmp);
		j++;
	}
	args[k] = NULL;
	*i = j;
	return (args);
}