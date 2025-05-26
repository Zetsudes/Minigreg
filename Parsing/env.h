#ifndef ENV_H
# define ENV_H

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;


#endif