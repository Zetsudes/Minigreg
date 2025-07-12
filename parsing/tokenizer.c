/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkantzer <lkantzer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:15:00 by lkantzer          #+#    #+#             */
/*   Updated: 2025/06/28 19:00:00 by lkantzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

/* ───────── helpers basiques ───────── */
static int is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == ';');
}

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'); 
}

/* ───────── builder ────────────────── */
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
	if (b->len + 1 >= b->cap && !builder_grow(b))
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

/* ───────── lexer workspace ────────── */
typedef enum e_state { GENERAL, IN_SQ, IN_DQ }	t_state;

typedef struct s_lexer
{
	char		**tok;
	size_t		len;
	size_t		cap;
	t_builder	w;
	t_state		st;
}	t_lexer;

/* ───────── tokens grow ───────────── */
static int	tokens_grow(t_lexer *lx)
{
	char	**new;
	size_t	i;

	if (lx->len + 2 < lx->cap)
		return (1);
	lx->cap *= 2;
	new = malloc(sizeof(char *) * lx->cap);
	if (!new)
		return (0);
	i = 0;
	while (i < lx->len)
	{
		new[i] = lx->tok[i];
		i++;
	}
	free(lx->tok);
	lx->tok = new;
	return (1);
}

/* ───────── ajouts élémentaires ───── */
static int	add_word(t_lexer *lx)
{
	if (!lx->w.len)
		return (1);
	if (!tokens_grow(lx))
		return (0);
	lx->tok[lx->len] = builder_take(&lx->w);
	if (!lx->tok[lx->len])
		return (0);
	lx->len++;
	return (1);
}

static int	add_operator(t_lexer *lx, const char *ln, size_t *i)
{
	size_t	oplen;
	char	*op;

	oplen = 1;
	if ((ln[*i] == '<' || ln[*i] == '>') && ln[*i + 1] == ln[*i])
		oplen = 2;
	op = ft_substr(ln, *i, oplen);
	if (!op || !tokens_grow(lx))
		return (free(op), 0);
	lx->tok[lx->len++] = op;
	*i += oplen;
	return (1);
}

/* ───────── handlers ───────────────── */
static int	handle_space(const char *ln, size_t *i, t_lexer *lx)
{
	if (lx->st == GENERAL && is_space(ln[*i]))
	{
		if (!add_word(lx))
			return (0);
		(*i)++;
		return (1);
	}
	return (2);
}

static int	handle_operator(const char *ln, size_t *i, t_lexer *lx)
{
	if (lx->st == GENERAL && is_operator(ln[*i]))
	{
		if (!add_word(lx) || !add_operator(lx, ln, i))
			return (0);
		return (1);
	}
	return (2);
}

static int  handle_paren(const char *ln, size_t *i, t_lexer *lx)
{
    if (lx->st == GENERAL && (ln[*i] == '(' || ln[*i] == ')'))
    {
        if (!add_word(lx) || !tokens_grow(lx))
            return (0);
        /* on stocke le caractère tout seul comme token                       */
        lx->tok[lx->len++] = ft_substr(ln, *i, 1);
        (*i)++;
        return (1);
    }
    return (2);
}

static int	handle_backslash(const char *ln, size_t *i, t_lexer *lx)
{
	if (ln[*i] == '\\' && lx->st != IN_SQ && ln[*i + 1])
	{
		if (!builder_push(&lx->w, '\\')
			|| !builder_push(&lx->w, ln[*i + 1]))
			return (-1);
		*i += 2;
		return (1);
	}
	return (2);
}

static int	handle_squote(const char *ln, size_t *i, t_lexer *lx)
{
	char	c;

	c = ln[*i];
	if (lx->st == GENERAL && c == '\'')
	{
		if (!builder_push(&lx->w, c))
			return (-1);
		lx->st = IN_SQ;
		(*i)++;
		return (1);
	}
	if (lx->st == IN_SQ && c == '\'')
	{
		if (!builder_push(&lx->w, c))
			return (-1);
		lx->st = GENERAL;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	handle_dquote(const char *ln, size_t *i, t_lexer *lx)
{
	char	c;

	c = ln[*i];
	if (lx->st == GENERAL && c == '"')
	{
		if (!builder_push(&lx->w, c))
			return (-1);
		lx->st = IN_DQ;
		(*i)++;
		return (1);
	}
	if (lx->st == IN_DQ && c == '"')
	{
		if (!builder_push(&lx->w, c))
			return (-1);
		lx->st = GENERAL;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	handle_quotes(const char *ln, size_t *i, t_lexer *lx)
{
	int	ret;

	ret = handle_squote(ln, i, lx);
	if (ret != 0)
		return (ret);
	ret = handle_dquote(ln, i, lx);
	if (ret != 0)
		return (ret);
	return (2);
}

/* ───────── étape de lexing ────────── */
static int	lex_step(const char *ln, size_t *i, t_lexer *lx)
{
	if (handle_space(ln, i, lx) != 2)
		return (handle_space(ln, i, lx));
	if (handle_paren(ln, i, lx)   != 2)
		return handle_paren(ln, i, lx);
	if (handle_operator(ln, i, lx) != 2)
		return (handle_operator(ln, i, lx));
	if (handle_backslash(ln, i, lx) != 2)
		return (handle_backslash(ln, i, lx));
	if (handle_quotes(ln, i, lx) != 2)
		return (handle_quotes(ln, i, lx));
	if (!builder_push(&lx->w, ln[*i]))
		return (0);
	(*i)++;
	return (1);
}

/* ───────── fonction publique ──────── */
char	**tokenize_line(char *line)
{
	t_lexer	lx;
	size_t	i;

	if (!line)
		return (NULL);
	lx.cap = 8;
	lx.len = 0;
	lx.st = GENERAL;
	lx.tok = malloc(sizeof(char *) * lx.cap);
	if (!lx.tok || !builder_init(&lx.w))
		return (free(lx.tok), NULL);
	i = 0;
	while (line[i])
		if (!lex_step(line, &i, &lx))
			return (free(lx.tok), free(lx.w.data), NULL);
	if (lx.st != GENERAL || !add_word(&lx))
		return (free(lx.tok), free(lx.w.data), NULL);
	lx.tok[lx.len] = NULL;
	return (lx.tok);
}
