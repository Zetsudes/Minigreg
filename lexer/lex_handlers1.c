/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_handlers1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:24:34 by marvin            #+#    #+#             */
/*   Updated: 2025/08/14 09:31:52 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_space(const char *ln, size_t *i, t_lexer *lx)
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

int	handle_operator(const char *ln, size_t *i, t_lexer *lx)
{
	if (lx->st == GENERAL && is_operator(ln[*i]))
	{
		if (!add_word(lx) || !add_operator(lx, ln, i))
			return (0);
		return (1);
	}
	return (2);
}

int	handle_paren(const char *ln, size_t *i, t_lexer *lx)
{
	if (lx->st == GENERAL && (ln[*i] == '(' || ln[*i] == ')'))
	{
		if (!add_word(lx) || !tokens_grow(lx))
			return (0);
		lx->tok[lx->len++] = ft_substr(ln, *i, 1);
		(*i)++;
		return (1);
	}
	return (2);
}

int	handle_backslash(const char *ln, size_t *i, t_lexer *lx)
{
	if (ln[*i] == '\\' && lx->st != IN_SQ && ln[*i + 1])
	{
		if (!builder_push(&lx->w, '\\') || !builder_push(&lx->w, ln[*i + 1]))
			return (-1);
		*i += 2;
		return (1);
	}
	return (2);
}
