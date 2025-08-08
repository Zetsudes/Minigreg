/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:27:11 by marvin            #+#    #+#             */
/*   Updated: 2025/07/21 08:27:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkantzer <lkantzer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:10:00 by lkantzer          #+#    #+#             */
/*   Updated: 2025/06/28 22:15:00 by lkantzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/expend.h"

int	expv_try_cases(const char *in, int *i, t_ctx *c)
{
	int	r;

	r = case_backslash(in, i, c);
	if (r != 0)
		return (r);
	r = case_single_quote(in, i, c);
	if (r != 0)
		return (r);
	r = case_double_quote(in, i, c);
	if (r != 0)
		return (r);
	r = case_dollar(in, i, c);
	if (r != 0)
		return (r);
	return (0);
}

int	expv_process_char(const char *in, int *i, t_ctx *c)
{
	int	ret;

	ret = expv_try_cases(in, i, c);
	if (ret != 0)
	{
		if (ret == -1)
			return (0);
		return (1);
	}
	if (append_char(c->out, in[*i]) == -1)
		return (0);
	(*i)++;
	return (1);
}

int	expand_core(const char *in, char **out, t_env *env)
{
	t_ctx	c;
	int		i;

	c.out = out;
	c.sq = 0;
	c.dq = 0;
	c.env = env;
	i = 0;
	while (in[i])
	{
		if (!expv_process_char(in, &i, &c))
			return (0);
	}
	return (1);
}

char	*expand_var(const char *input, t_env *env)
{
	char	*res;

	if (!input)
		return (NULL);
	res = NULL;
	if (!expand_core(input, &res, env))
		res = ft_strdup("");
	return (res);
}
