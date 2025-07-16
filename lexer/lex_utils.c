#include "../include/minishell.h"

int is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == ';');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'); 
}

int	tokens_grow(t_lexer *lx)
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