/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/25 00:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

/* ───────── helpers ───────── */
static int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/* ───────── builder ───────── */
typedef struct s_builder
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_builder;

static int	builder_init(t_builder *b)
{
	b->cap = 16;
	b->len = 0;
	b->data = malloc(b->cap);
	if (!b->data)
		return (0);
	b->data[0] = '\0';
	return (1);
}

static int	builder_grow(t_builder *b)
{
	char	*new;
	size_t	i;

	b->cap *= 2;
	new = malloc(b->cap);
	if (!new)
		return (0);
	i = 0;
	while (i < b->len)
	{
		new[i] = b->data[i];
		i++;
	}
	free(b->data);
	b->data = new;
	return (1);
}

static int	builder_push(t_builder *b, char c)
{
	if (!b->data && !builder_init(b))
		return (0);
	if (b->len + 1 >= b->cap)
		if (!builder_grow(b))
			return (0);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (1);
}

static char	*builder_take(t_builder *b)
{
	char	*out;

	out = b->data;
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
	return (out);
}

/* ───────── états ───────── */
typedef enum e_state
{
	GENERAL,
	IN_SQ,
	IN_DQ
}	t_state;

/* ───────── tableau tokens ───────── */
static int	grow_tokens(char ***arr, size_t *cap, size_t len)
{
	char	**new;
	size_t	i;

	if (len + 2 < *cap)
		return (1);
	*cap *= 2;
	new = malloc(sizeof(char *) * (*cap));
	if (!new)
		return (0);
	i = 0;
	while (i < len)
	{
		new[i] = (*arr)[i];
		i++;
	}
	free(*arr);
	*arr = new;
	return (1);
}

/* ───────── lexer ───────── */
char	**tokenize_line(char *line)
{
	size_t		i;
	size_t		len;
	size_t		cap;
	char		**tok;
	t_builder	w;
	t_state		st;

	i = 0;
	len = 0;
	cap = 8;
	tok = malloc(sizeof(char *) * cap);
	if (!tok || !builder_init(&w))
		return (NULL);
	st = GENERAL;
	while (line[i])
	{
		if (st == GENERAL && is_space(line[i]))
		{
			if (w.len && !grow_tokens(&tok, &cap, len))
				return (NULL);
			if (w.len)
				tok[len++] = builder_take(&w);
			i++;
			continue ;
		}
		if (st == GENERAL && is_operator(line[i]))
		{
			if (w.len && !grow_tokens(&tok, &cap, len))
				return (NULL);
			if (w.len)
				tok[len++] = builder_take(&w);
			if ((line[i] == '<' || line[i] == '>') && line[i + 1] == line[i])
			{
				if (!grow_tokens(&tok, &cap, len))
					return (NULL);
				tok[len++] = ft_substr(line, i, 2);
				i += 2;
			}
			else
			{
				if (!grow_tokens(&tok, &cap, len))
					return (NULL);
				tok[len++] = ft_substr(line, i, 1);
				i++;
			}
			continue ;
		}
		if ((st == GENERAL || st == IN_DQ) && line[i] == '\\'
			&& (line[i + 1] == '\'' || line[i + 1] == '"' || line[i + 1] == '\\'
				|| is_space(line[i + 1]) || line[i + 1] == '$'))
		{
			if (!builder_push(&w, line[i + 1]))
				return (NULL);
			i += 2;
			continue ;
		}
		if ((st == GENERAL && line[i] == '\'') || (st == IN_SQ && line[i] == '\''))
		{
			st = (st == GENERAL) ? IN_SQ : GENERAL;
			i++;
			continue ;
		}
		if ((st == GENERAL && line[i] == '"') || (st == IN_DQ && line[i] == '"'))
		{
			st = (st == GENERAL) ? IN_DQ : GENERAL;
			i++;
			continue ;
		}
		if (!builder_push(&w, line[i]))
			return (NULL);
		i++;
	}
	if (st != GENERAL)
		return (free(w.data), NULL);
	if (w.len && grow_tokens(&tok, &cap, len))
		tok[len++] = builder_take(&w);
	tok[len] = NULL;
	return (tok);
}
