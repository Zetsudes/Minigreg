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

#include "../include/minishell.h"

/* -------------------------------------------------------------------------- */
/*  Helpers : append_char / append_str                                         */
/* -------------------------------------------------------------------------- */
static int	append_char(char **dst, char c)
{
	char	*new;
	size_t	len;

	len = 0;
	if (*dst)
		len = ft_strlen(*dst);
	new = malloc(len + 2);
	if (!new)
	{
		if (*dst)
		{
			free(*dst);
			*dst = NULL;
		}
		return (-1);
	}
	if (*dst)
	{
		ft_memcpy(new, *dst, len);
		free(*dst);
	}
	new[len] = c;
	new[len + 1] = '\0';
	*dst = new;
	return (1);
}

static int	free_and_fail(char **p)
{
	if (*p)
	{
		free(*p);
		*p = NULL;
	}
	return (-1);
}

static int	append_str(char **dst, const char *add)
{
	size_t	len;
	size_t	al;
	char	*new;

	if (!add)
		return (1);
	len = *dst ? ft_strlen(*dst) : 0;
	al = ft_strlen(add);
	new = malloc(len + al + 1);
	if (!new)
		return (free_and_fail(dst));
	if (*dst)
	{
		ft_memcpy(new, *dst, len);
		free(*dst);
	}
	ft_memcpy(new + len, add, al + 1);
	*dst = new;
	return (1);
}

/* -------------------------------------------------------------------------- */
/*  Gestion du $VARIABLE / $?                                                  */
/* -------------------------------------------------------------------------- */
static int	handle_dollar(const char *in, int *i, char **out, t_env *env)
{
	char	*var;
	char	*val;
	int		start;

	if (in[*i] == '?')
	{
		val = get_env_value(env, "?");
		if (!val)
			val = "0";
		(*i)++;                       /* saute le caractère '?' */
		return (append_str(out, val));
	}
	if (!ft_isalpha(in[*i]) && in[*i] != '_')
		return (append_char(out, '$'));
	start = *i;
	while (in[*i] && (ft_isalnum(in[*i]) || in[*i] == '_'))
		(*i)++;
	var = ft_substr(in, start, *i - start);
	if (!var)
		return (-1);
	val = get_env_value(env, var);
	free(var);
	if (!val)
		val = "";
	return (append_str(out, val));
}

/* -------------------------------------------------------------------------- */
/*  Contexte d’expansion                                                      */
/* -------------------------------------------------------------------------- */
typedef struct s_ctx
{
	char	**out;
	int		sq;
	int		dq;
	t_env	*env;
}	t_ctx;

/* -------------------------------------------------------------------------- */
/*  Handlers élémentaires                                                     */
/* -------------------------------------------------------------------------- */
static int  dq_backslash(const char *in, int *i, t_ctx *c)
{
    char next = in[*i + 1];

    if (next == '$' || next == '`' || next == '"' ||
        next == '\\' || next == '\n')
    {
        if (append_char(c->out, next) == -1)
            return (-1);
        *i += 2;
        return (1);
    }
    return (0);
}

static int  case_backslash(const char *in, int *i, t_ctx *c)
{
    if (c->sq || in[*i] != '\\' || !in[*i + 1])
        return (0);
    if (c->dq)
    {
        int r = dq_backslash(in, i, c);
        if (r)
            return (r);
        return (0);
    }
    if (append_char(c->out, in[*i + 1]) == -1)
        return (-1);
    *i += 2;
    return (1);
}

static int	case_single_quote(const char *in, int *i, t_ctx *c)
{
	if (in[*i] != '\'' || c->dq)
		return (0);
	c->sq = !c->sq;
	(*i)++;
	return (1);
}

static int	case_double_quote(const char *in, int *i, t_ctx *c)
{
	if (c->sq || in[*i] != '"')
		return (0);
	/* Ignore un guillemet précédé d'un backslash hors double-quotes */
	if (*i > 0 && in[*i - 1] == '\\' && c->dq == 0)
		return (0);
	c->dq = !c->dq;
	(*i)++;
	return (1);
}

static int	case_dollar(const char *in, int *i, t_ctx *c)
{
	int	ret;

	if (c->sq || in[*i] != '$')
		return (0);
	(*i)++;
	ret = handle_dollar(in, i, c->out, c->env);
	if (ret != 1)
		return (-1);
	return (1);
}

/* -------------------------------------------------------------------------- */
/*  Essaye chaque handler                                                     */
/* -------------------------------------------------------------------------- */
static int	expv_try_cases(const char *in, int *i, t_ctx *c)
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

static int	expv_process_char(const char *in, int *i, t_ctx *c)
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

/* -------------------------------------------------------------------------- */
/*  Boucle principale                                                         */
/* -------------------------------------------------------------------------- */
static int	expand_core(const char *in, char **out, t_env *env)
{
	t_ctx	c;
	int		i;

	c.out = out;
	c.sq = 0;
	c.dq = 0;
	c.env = env;
	i = 0;
	while (in[i])
		if (!expv_process_char(in, &i, &c))
			return (0);
	return (1);
}

/* -------------------------------------------------------------------------- */
/*  API publique                                                              */
/* -------------------------------------------------------------------------- */
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
