
#ifndef PARSER_H
# define PARSER_H

# include "../greg_lib/libft.h"

// Structure d'une commande
typedef struct s_cmd {
    char **args;              // arguments (ex: ["echo", "hello"])
    char *infile;             // fichier d'entrée (<)
    char *outfile;            // fichier de sortie (>)
    int append;               // 1 si >>
    int heredoc;              // 1 si <<
    struct s_cmd *next;       // pour les pipes
} t_cmd;

char    **tokenize_line(char *line);
t_cmd	*parse_tokens(char **tokens);

#endif
