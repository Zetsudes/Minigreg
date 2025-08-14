/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <marvin@student.42.fr>                   +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 21:35:00 by 42                #+#    #+#             */
/*   Updated: 2025/08/13 21:35:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "lexer.h"
# include "env.h"

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	int				redir_error;
	int				cmd_separator;
	struct s_cmd	*next;
}	t_cmd;

t_cmd	*parse_tokens(char **tokens, t_env *env);
void	free_cmd_list(t_cmd *cmd_list);
int		print_syntax_error(const char *tok);
int		handle_pipe_errors(char **tk, int i, t_cmd *cmd);
t_cmd	*process_segment(char **tk, int *i, t_env *env);
int		process_redirs(t_cmd *cmd, char **tk, int *i);
char	**copy_args(char **tokens, int *i, t_cmd *cmd, t_env *env);
int		is_token_operator(const char *token);
int		ft_strcmp(const char *s1, const char *s2);
t_cmd	*init_cmd(void);
int		set_redirection(t_cmd *cmd, char *op, char *file);
char	*expand_var(const char *arg, t_env *env);
int		is_sep(const char *s);
int		token_is_quoted(const char *s);
char	*process_token(char *tok, t_env *env);
int		count_effective_args(char **tk, int j, t_env *env);
void	free_args_partial(char **args, int k);
void	skip_to_sep(int *j, char **tokens);
void	add_piece(char **args, int *k, char *token, t_env *env);

#endif
