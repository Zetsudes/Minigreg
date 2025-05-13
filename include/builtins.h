

#ifndef BUILTINS_H
# define BUILTINS_H


typedef struct s_builtin {
    char *name;
    void (*func)(char **args);
} t_builtin;

int		cd(char **args);
int     pd();
int		pwd(void);
int     is_builtin(t_cmd *cmd);
void		handle_builtin(t_cmd *cmd);


#endif