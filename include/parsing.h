
#ifndef PARSING_H
# define PARSING_H

# include "lexer.h"
# include "env.h"

typedef struct s_cmd
{
	char			**args;        /* ["echo", "hello"]            */
	char			*infile;       /* <                            */
	char			*outfile;      /* > ou >>                      */
	int				append;        /* 1 si >>                      */
	int				heredoc;       /* 1 si <<                      */
	int				cmd_separator; /* 1 si ‘;’                     */
	struct s_cmd	*next;         /* pipe suivant                 */
}	t_cmd;

t_cmd	*parse_tokens(char **tokens, t_env *env);
void	free_cmd_list(t_cmd *cmd_list);
int		print_syntax_error(const char *tok);
int		handle_pipe_errors(char **tk, int i, t_cmd *cmd);
int		process_segment(t_cmd **head, t_cmd **cur,
						char **tk, int *i, t_env *env);
int		process_redirs(t_cmd *cmd, char **tk, int *i);
char	**copy_args(char **tokens, int *i, t_env *env);
int		is_token_operator(const char *token);
int		ft_strcmp(const char *s1, const char *s2);
t_cmd	*init_cmd(void);
int		set_redirection(t_cmd *cmd, char *op, char *file);
char	*expand_var(const char *arg, t_env *env);

#endif