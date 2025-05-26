#include "parser.h"

int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/* Fonction pour redimensionner le tableau de tokens */
char	**resize_tokens_array(char **tokens, int old_size, int new_size)
{
	char	**new_tokens;
	int		i;

	new_tokens = malloc(sizeof(char *) * new_size);
	if (!new_tokens)
		return (NULL);
	i = 0;
	while (i < old_size)
	{
		new_tokens[i] = tokens[i];
		i++;
	}
	free(tokens);
	return (new_tokens);
}

/* Extraction d'un token entre quotes, avec gestion des backslashes */
char	*extract_quoted_token(char *line, int *i)
{
	char	quote;
	int		start;
	char	*token;
	char	*cleaned_token;
	int		j;
	int		k;

	quote = line[*i];
	start = ++(*i);
	j = 0;

	while (line[*i] && (line[*i] != quote || (line[*i - 1] == '\\' && line[*i] == quote)))
		(*i)++;
	if (line[*i] != quote)
	{
		printf("❌ Erreur : Quote non fermée\n");
		return (NULL);
	}
	token = ft_substr(line, start, *i - start);
	cleaned_token = malloc(sizeof(char) * (ft_strlen(token) + 1));
	if (!cleaned_token)
		return (NULL);
	
	j = 0;
	k = 0;
	while (token[j])
	{
		if (token[j] == '\\' && (token[j + 1] == quote || token[j + 1] == '\\'))
			j++;
		cleaned_token[k++] = token[j++];
	}
	cleaned_token[k] = '\0';
	free(token);

	(*i)++;
	return (cleaned_token);
}

char	*extract_operator_token(char *line, int *i)
{
	char	*token;

	if ((line[*i] == '<' && line[*i + 1] == '<') || (line[*i] == '>' && line[*i + 1] == '>'))
	{
		token = ft_substr(line, *i, 2);
		*i += 2;
	}
	else
	{
		token = ft_substr(line, *i, 1);
		(*i)++;
	}
	return (token);
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
	int		i = 0;
	int		j = 0;
	int		size = 10;
	char	**tokens;

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
		if (j >= size - 1)
		{
			size *= 2;
			tokens = resize_tokens_array(tokens, j, size);
			if (!tokens)
				return (NULL);
		}
		if (line[i] == '"' || line[i] == '\'')
			tokens[j++] = extract_quoted_token(line, &i);
		else if (is_operator(line[i]))
			tokens[j++] = extract_operator_token(line, &i);
		else
			tokens[j++] = extract_simple_token(line, &i);
	}
	tokens[j] = NULL;
	return (tokens);
}
