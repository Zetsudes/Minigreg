#include "../include/minishell.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
static char *strip_quotes(char *str)
{
	size_t len = strlen(str);

	if ((str[0] == '\'' && str[len - 1] == '\'') ||
		(str[0] == '"' && str[len - 1] == '"'))
	{
		char *new_str = strndup(str + 1, len - 2);
		free(str);
		return new_str;
	}
	return str; // Pas de quotes ou quotes mal appariées, on renvoie tel quel
}*/


/* Extraction nom variable à partir d'une position (dans expand_var) */
int extract_varname(const char *input, int *pos, char **varname)
{
	int start = *pos;
	if (!input[start] || (!isalpha(input[start]) && input[start] != '_'))
		return 0;
	(*pos)++;
	while (input[*pos] && (isalnum(input[*pos]) || input[*pos] == '_'))
		(*pos)++;
	int len = *pos - start;
	*varname = strndup(input + start, len);
	return (*varname != NULL);
}

/* Ajoute un caractère à une string dynamique */
static int append_char(char **str, char c)
{
	size_t len = *str ? strlen(*str) : 0;
	char *new_str = realloc(*str, len + 2);
	if (!new_str)
	{
		free(*str);
		*str = NULL;
		return 0;
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	*str = new_str;
	return 1;
}

/* Ajoute une chaîne à une string dynamique */
static int append_str(char **str, const char *to_add)
{
	if (!to_add)
		return 1;
	size_t len = *str ? strlen(*str) : 0;
	size_t add_len = strlen(to_add);
	char *new_str = realloc(*str, len + add_len + 1);
	if (!new_str)
	{
		free(*str);
		*str = NULL;
		return 0;
	}
	memcpy(new_str + len, to_add, add_len);
	new_str[len + add_len] = '\0';
	*str = new_str;
	return 1;
}

/* Fonction principale d'expansion qui traite les quotes et les variables */
char *expand_var(const char *input, t_env *env)
{
	char *result = NULL;
	int i = 0;
	int in_single = 0;
	int in_double = 0;

	if (!input)
		return NULL;

	while (input[i])
	{
		if (input[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			i++; // On avance sans copier la quote
		}
		else if (input[i] == '"' && !in_single)
		{
			in_double = !in_double;
			i++; // On avance sans copier la quote
		}
		else if (input[i] == '$' && !in_single)
		{
			i++;
			if (input[i] == '?')
			{
				append_str(&result, "0"); // TODO: mettre le vrai code de sortie
				i++;
			}
			else
			{
				char *varname = NULL;
				if (!extract_varname(input, &i, &varname))
					append_char(&result, '$'); // Pas de var valide, on garde '$'
				else
				{
					char *val = get_env_value(env, varname);
					free(varname);
					if (val)
						append_str(&result, val);
				}
			}
		}
		else
		{
			append_char(&result, input[i]);
			i++;
		}
		if (!result) // erreur malloc
			break;
	}

	return result;
}
