#ifndef EXPAND_H
# define EXPAND_H

# include "../include/minishell.h"

typedef struct s_ctx
{
	char	**out;
	int		sq;
	int		dq;
	t_env	*env;
}	t_ctx;

char	*expand_var(const char *input, t_env *env);
int	 append_char(char **dst, char c);
int	 append_str(char **dst, const char *add);
int	 free_and_fail(char **p);
int	 handle_dollar(const char *in, int *i, char **out, t_env *env);
int	 case_dollar(const char *in, int *i, t_ctx *c);
int	 dq_backslash(const char *in, int *i, t_ctx *c);
int	 case_backslash(const char *in, int *i, t_ctx *c);
int	 case_single_quote(const char *in, int *i, t_ctx *c);
int	 case_double_quote(const char *in, int *i, t_ctx *c);
int	 expv_try_cases(const char *in, int *i, t_ctx *c);
int	 expv_process_char(const char *in, int *i, t_ctx *c);
int	 expand_core(const char *in, char **out, t_env *env);

#endif