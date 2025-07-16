#include "../include/minishell.h"

int	builder_init(t_builder *b)
{
	b->cap = 16;
	b->len = 0;
	b->data = malloc(b->cap);
	if (!b->data)
		return (0);
	b->data[0] = '\0';
	return (1);
}

int	builder_grow(t_builder *b)
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

int	builder_push(t_builder *b, char c)
{
	if (!b->data && !builder_init(b))
		return (0);
	if (b->len + 1 >= b->cap && !builder_grow(b))
		return (0);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (1);
}

char	*builder_take(t_builder *b)
{
	char	*out;

	out = b->data;
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
	return (out);
}