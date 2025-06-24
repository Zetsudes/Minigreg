/* ************************************************************************** */
/*                               expand_var.c                                 */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

/* ── helpers env ─────────────────────────────────────────────────────────── */

static int	is_start(char c)          { return (ft_isalpha(c) || c == '_'); }
static int	is_varc(char c)           { return (ft_isalnum(c) || c == '_'); }

static char	*lookup(t_env *e, char *k, size_t n)
{
	while (e)
	{
		if (!ft_strncmp(e->key, k, n) && e->key[n] == '\0')
			return (e->value);
		e = e->next;
	}
	return (NULL);
}

/* ── buffer dynamique ───────────────────────────────────────────────────── */

typedef struct s_buf { char *s; size_t len; size_t cap; }	t_buf;

static int	buf_init(t_buf *b)
{
	b->cap = 32;
	b->len = 0;
	b->s = malloc(b->cap);
	if (!b->s)
		return (0);
	b->s[0] = '\0';
	return (1);
}

static int	buf_grow(t_buf *b, size_t need)
{
	size_t	newcap = b->cap;
	char	*new;

	while (b->len + need + 1 >= newcap)
		newcap *= 2;
	new = malloc(newcap);
	if (!new)
		return (0);
	memcpy(new, b->s, b->len + 1);
	free(b->s);
	b->s = new;
	b->cap = newcap;
	return (1);
}

static int	add_char(t_buf *b, char c)
{
	if (!buf_grow(b, 1))
		return (0);
	b->s[b->len++] = c;
	b->s[b->len] = '\0';
	return (1);
}

static int	add_str(t_buf *b, const char *s)
{
	size_t	n;

	if (!s)
		return (1);
	n = ft_strlen(s);
	if (!buf_grow(b, n))
		return (0);
	memcpy(b->s + b->len, s, n);
	b->len += n;
	b->s[b->len] = '\0';
	return (1);
}

static size_t	extract(const char *s, size_t i, char **name)
{
	size_t	start;

	if (s[i] == '?')
	{
		*name = ft_strdup("?");
		return (1);
	}
	if (!is_start(s[i]))
		return (0);
	start = i;
	while (s[i] && is_varc(s[i]))
		i++;
	*name = ft_substr(s, start, i - start);
	return (i - start);
}

/* ── handlers ponctuels ─────────────────────────────────────────────────── */

static int  handle_backslash(const char *s, size_t *i, t_buf *o)
{
    /* cas  \ $   ou   \\ $  → on veut garder '\' puis traiter '$'   */
    if (s[*i + 1] == '$')
    {
        if (!add_char(o, '\\'))       /* on émet le backslash        */
            return (0);
        (*i)++;                       /* on saute SEULEMENT '\'      */
        return (1);                   /* la boucle verra alors '$'   */
    }
    /* cas  \\  -> un seul '\' dans la sortie ---------------------- */
    if (s[*i + 1] == '\\')
    {
        if (!add_char(o, '\\'))
            return (0);
        (*i) += 2;                    /* on saute les 2 backslashes  */
        return (1);
    }
    /* cas générique : \x où x n'est ni $ ni \ --------------------- */
    if (!add_char(o, s[*i + 1]))
        return (0);
    (*i) += 2;
    return (1);
}



static int	handle_dollar(const char *s, size_t *i, t_buf *o, t_env *env)
{
	char	*name;
	char	*val;

	(*i)++;
	if (!( *i += extract(s, *i, &name)))
		return (add_char(o, '$'));
	val = (name[0] == '?' && name[1] == '\0')
		? ft_itoa(get_exit_status())
		: lookup(env, name, ft_strlen(name));
	if (!add_str(o, val ? val : ""))
		return (free(name), 0);
	if (name[0] == '?' && name[1] == '\0')
		free(val);
	free(name);
	return (1);
}

/* ── fonction principale ───────────────────────────────────────────────── */

char	*expand_var(const char *str, t_env *env)
{
	t_buf	out;
	size_t	i = 0;

	if (!str || !buf_init(&out))
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\'
			&& (str[i + 1] == '$' || str[i + 1] == '\\'))
		{
			if (!handle_backslash(str, &i, &out))
				return (free(out.s), NULL);
		}
		else if (str[i] == '$')
		{
			if (!handle_dollar(str, &i, &out, env))
				return (free(out.s), NULL);
		}
		else
		{
			if (!add_char(&out, str[i++]))
				return (free(out.s), NULL);
		}
	}
	if (out.len == ft_strlen(str) && !strchr(str, '$') && !strchr(str, '\\'))
	{
		free(out.s);
		return (ft_strdup((char *)str));
	}
	return (out.s);
}