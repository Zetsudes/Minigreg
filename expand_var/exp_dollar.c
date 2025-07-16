#include "../include/expend.h"

int	handle_dollar(const char *in, int *i, char **out, t_env *env)
{
	char	*var;
	char	*val;
	int		start;

	if (in[*i] == '?')
	{
		val = get_env_value(env, "?");
		if (!val)
			val = "0";
		(*i)++;
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

int	case_dollar(const char *in, int *i, t_ctx *c)
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