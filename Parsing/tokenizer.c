#include "parser.h"

int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

char *extract_quoted_token(char *line, int *i)
{
	char	quote = line[*i];
	int		start = ++(*i);

	while (line[*i] && line[*i] != quote)
		(*i)++;
	
	char *token = ft_substr(line, start, *i - start);
	if (line[*i])
		(*i)++;
	return token;
}

char *extract_operator_token(char *line, int *i)
{
	if ((line[*i] == '<' && line[*i + 1] == '<') ||
		(line[*i] == '>' && line[*i + 1] == '>'))
	{
		char *token = ft_substr(line, *i, 2);
		*i += 2;
		return token;
	}
	else
	{
		char *token = ft_substr(line, *i, 1);
		(*i)++;
		return token;
	}
}

char *extract_simple_token(char *line, int *i)
{
	int start = *i;

	while (line[*i] && !is_space(line[*i]) && !is_operator(line[*i]) && line[*i] != '\'' && line[*i] != '"')
		(*i)++;

	return ft_substr(line, start, *i - start);
}

char **tokenize_line(char *line)
{
	int i = 0;
	int j = 0;
	int size = 10;
	char **tokens = malloc(sizeof(char*) * size);

	if (!tokens)
		return NULL;
	while (line[i])
	{
		if (is_space(line[i]))
		{
			i++;
			continue;
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
			tokens = realloc(tokens, sizeof(char*) * size); // je sais pas si realloc est autorisé 
		}
	}
	tokens[j] = NULL;
	return tokens;
}
