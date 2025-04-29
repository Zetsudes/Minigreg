#include "parser.h"

int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

char	*extract_quoted_token(char *line, int *i)
{
	char	quote;
	int		start;
	char	*token;

	quote = line[*i];
	start = ++(*i);
	while (line[*i] && line[*i] != quote)
		(*i)++;
	token = ft_substr(line, start, *i - start);
	if (line[*i])
		(*i)++;
	return (token);
}

char	*extract_operator_token(char *line, int *i)
{
	char	*token;
	char	*token;

	if ((line[*i] == '<' && line[*i + 1] == '<') || (line[*i] == '>' && line[*i
			+ 1] == '>'))
	{
		token = ft_substr(line, *i, 2);
		*i += 2;
		return (token);
	}
	else
	{
		token = ft_substr(line, *i, 1);
		(*i)++;
		return (token);
	}
}

char	*extract_simple_token(char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && !is_space(line[*i]) && !is_operator(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

char	**tokenize_line(char *line)
{
	int		i;
	int		j;
	int		size;
	char	**tokens;

	i = 0;
	j = 0;
	size = 10;
	tokens = malloc(sizeof(char *) * size);
	if (!tokens)
		return (NULL);
	while (line[i])
	{
		if (is_space(line[i]))
		{
			i++;
			continue ;
		}
		if (line[i] == '"' || line[i] == '\'')
			tokens[j++] = extract_quoted_token(line, &i);
		else if (is_operator(line[i]))
			tokens[j++] = extract_operator_token(line, &i);
		else
			tokens[j++] = extract_simple_token(line, &i);
		if (j >= size - 1)
		{
			size *= 2;
			tokens = realloc(tokens, sizeof(char *) * size); // non autorisé
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
