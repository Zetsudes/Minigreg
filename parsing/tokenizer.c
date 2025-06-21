#include "../include/minishell.h"
#include <stdlib.h>
#include <stdio.h> // pour printf en cas d'erreur

int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/* Redimensionne le tableau de tokens */
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

/* Extraction d'un token entre quotes : on garde les quotes */
char	*extract_quoted_token(char *line, int *i)
{
	char	quote = line[*i];
	int		start = *i;
	int		j;

	j = start + 1;
	while (line[j])
	{
		if (line[j] == quote)
			break;
		// On ignore les backslashs ici, on gère ça à l'expansion
		j++;
	}
	if (line[j] != quote)
	{
		printf("❌ Erreur : Quote non fermée\n");
		return (NULL);
	}
	// On prend la chaîne entre quotes INCLUSES
	char *token = ft_substr(line, start, j - start + 1);
	if (!token)
		return (NULL);
	*i = j + 1; // Avance après la quote fermante
	return (token);
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
	int	start = *i;

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
	char	**tokens = malloc(sizeof(char *) * size);

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
		if (line[i] == '\'' || line[i] == '"')
		{
			tokens[j] = extract_quoted_token(line, &i);
			if (!tokens[j])
			{
				for (int k = 0; k < j; k++)
					free(tokens[k]);
				free(tokens);
				return (NULL);
			}
			j++;
		}
		else if (is_operator(line[i]))
		{
			tokens[j] = extract_operator_token(line, &i);
			if (!tokens[j])
			{
				for (int k = 0; k < j; k++)
					free(tokens[k]);
				free(tokens);
				return (NULL);
			}
			j++;
		}
		else
		{
			tokens[j] = extract_simple_token(line, &i);
			if (!tokens[j])
			{
				for (int k = 0; k < j; k++)
					free(tokens[k]);
				free(tokens);
				return (NULL);
			}
			j++;
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
