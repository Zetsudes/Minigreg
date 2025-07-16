#include "../include/minishell.h"

int	handle_squote(const char *ln, size_t *i, t_lexer *lx)
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

int	handle_dquote(const char *ln, size_t *i, t_lexer *lx)
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

int	handle_quotes(const char *ln, size_t *i, t_lexer *lx)
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