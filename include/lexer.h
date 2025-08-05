#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>

typedef struct s_builder
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_builder;

typedef enum e_state
{
    GENERAL,
    IN_SQ,
    IN_DQ
}   t_state;

typedef struct s_lexer
{
	char		**tok;
	size_t		len;
	size_t		cap;
	t_builder	w;
	t_state		st;
}	t_lexer;


char	**tokenize_line(char *line);
int		is_operator(char c);
int		is_space(char c);
int		tokens_grow(t_lexer *lx);
int		builder_init(t_builder *b);
int		builder_grow(t_builder *b);
int		builder_push(t_builder *b, char c);
char	*builder_take(t_builder *b);
int		add_word(t_lexer *lx);
int		add_operator(t_lexer *lx, const char *ln, size_t *i);
int		handle_space(const char *ln, size_t *i, t_lexer *lx);
int		handle_operator(const char *ln, size_t *i, t_lexer *lx);
int		handle_paren(const char *ln, size_t *i, t_lexer *lx);
int		handle_backslash(const char *ln, size_t *i, t_lexer *lx);
int		handle_squote(const char *ln, size_t *i, t_lexer *lx);
int		handle_dquote(const char *ln, size_t *i, t_lexer *lx);
int		handle_quotes(const char *ln, size_t *i, t_lexer *lx);
int		lex_step(const char *ln, size_t *i, t_lexer *lx);
int		is_empty_quotes(const char *tok);

#endif
