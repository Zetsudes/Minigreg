/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:21:26 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 09:31:28 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_word(t_lexer *lx)
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

int	add_operator(t_lexer *lx, const char *ln, size_t *i)
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
