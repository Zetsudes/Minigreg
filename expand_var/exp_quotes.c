#include "../include/expend.h"

int  dq_backslash(const char *in, int *i, t_ctx *c)
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

int  case_backslash(const char *in, int *i, t_ctx *c)
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


int	case_single_quote(const char *in, int *i, t_ctx *c)
{
	if (in[*i] != '\'' || c->dq)
		return (0);
	c->sq = !c->sq;
	(*i)++;
	return (1);
}

int	case_double_quote(const char *in, int *i, t_ctx *c)
{
	if (c->sq || in[*i] != '"')
		return (0);
	if (*i > 0 && in[*i - 1] == '\\' && c->dq == 0)
		return (0);
	c->dq = !c->dq;
	(*i)++;
	return (1);
}