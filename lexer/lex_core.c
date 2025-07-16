#include "../include/minishell.h"

int	lex_step(const char *ln, size_t *i, t_lexer *lx)
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