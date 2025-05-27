#include "parser.h"
#include <stdio.h> // For printf used in error messages

int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int is_alpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_alnum(char c)
{
	return (is_alpha(c) || (c >= '0' && c <= '9'));
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

	// Find the closing quote based on quote type
	if (quote == '\'')
	{
		while (line[*i] && line[*i] != quote)
			(*i)++;
	}
	else // For double quotes
	{
		while (line[*i] && (line[*i] != quote || (line[*i - 1] == '\\')))
			(*i)++;
	}

	if (line[*i] != quote)
	{
		printf("❌ Erreur : Quote non fermée\n");
		return (NULL);
	}

	token = ft_substr(line, start, *i - start);
	if (!token) // Check if ft_substr failed
		return (NULL);

	// Allocate memory for cleaned_token. 
	// For single quotes, it's the same length as token.
	// For double quotes, it can be shorter due to escape sequences, but ft_strlen(token) is a safe upper bound.
	cleaned_token = malloc(sizeof(char) * (ft_strlen(token) + 1));
	if (!cleaned_token)
	{
		free(token);
		return (NULL);
	}

	j = 0; // Index for token
	k = 0; // Index for cleaned_token

	if (quote == '\'')
	{
		// For single quotes, copy verbatim
		while (token[j])
		{
			cleaned_token[k++] = token[j++];
		}
	}
	else // For double quotes
	{
		while (token[j])
		{
			if (token[j] == '\\' && token[j + 1] && (token[j + 1] == '$' || token[j + 1] == '"' || token[j + 1] == '\\'))
			{
				j++; // Skip the backslash
				cleaned_token[k++] = token[j++]; // Copy the escaped character
			}
			else
			{
				cleaned_token[k++] = token[j++];
			}
		}
	}
	cleaned_token[k] = '\0';
	free(token);

	(*i)++; // Move past the closing quote
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
	if (line[start] == '$')
	{
		(*i)++; // Move past '$'
		if (line[*i] == '?') // Case: $?
		{
			(*i)++; // Move past '?'
		}
		else if (is_alpha(line[*i]) || line[*i] == '_') // Case: $VAR or $_VAR
		{
			(*i)++; // Move past the first char of VAR
			while (line[*i] && (is_alnum(line[*i]) || line[*i] == '_'))
			{
				(*i)++;
			}
		}
		// Else: it's just '$' or '$' followed by non-var character. 
		// *i is already incremented past '$', so the length is *i - start.
		// If $ is followed by non-var char, *i remains start+1.
		// If $ is at end of line, *i remains start+1.
	}
	else
	{
		// Original logic for non-'$' prefixed tokens
		while (line[*i] && !is_space(line[*i]) && !is_operator(line[*i])
			   && line[*i] != '\'' && line[*i] != '"' && line[*i] != '$') // Added '$' to delimiters
			(*i)++;
	}
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
		{
			tokens[j] = extract_quoted_token(line, &i);
			if (tokens[j] == NULL) // Error from extract_quoted_token
			{
				// Free all tokens allocated so far in this call to tokenize_line
				for (int k = 0; k < j; k++) // k < j because tokens[j] is the one that failed (is NULL)
				{
					free(tokens[k]);
				}
				free(tokens); // Free the array of pointers
				return (NULL); // Indicate tokenization failure
			}
			j++; // Increment j only if token extraction was successful
		}
		else if (is_operator(line[i]))
		{
			tokens[j] = extract_operator_token(line, &i);
			// Assuming extract_operator_token also might fail and return NULL,
			// and ft_substr can return NULL if malloc fails.
			if (tokens[j] == NULL)
			{
				for (int k = 0; k < j; k++)
				{
					free(tokens[k]);
				}
				free(tokens);
				return (NULL);
			}
			j++;
		}
		else
		{
			tokens[j] = extract_simple_token(line, &i);
			// Assuming extract_simple_token also might fail and return NULL,
			// and ft_substr can return NULL if malloc fails.
			if (tokens[j] == NULL)
			{
				for (int k = 0; k < j; k++)
				{
					free(tokens[k]);
				}
				free(tokens);
				return (NULL);
			}
			j++;
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
