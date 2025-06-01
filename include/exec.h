/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:47:32 by zamohame          #+#    #+#             */
/*   Updated: 2025/05/02 18:02:26 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_cmd {
    char **args;              // arguments (ex: ["echo", "hello"])
    char *infile;             // fichier d'entrée (<)
    char *outfile;            // fichier de sortie (>)
    int append;               // 1 si >>
    int heredoc;              // 1 si <<
    struct s_cmd *next;       // pour les pipes
} t_cmd;

/********** PARSING ***********/ 
char		**tokenize_line(char *line);
t_cmd		*parse_tokens(char **tokens);
void		free_cmd_list(t_cmd *cmd_list);

/********** PATH ***********/ 
char		*find_path(char **dir, char *cmd);
char		*get_path(char *cmd, char **env);

/********** EXEC ***********/ 
void		execute_command(t_cmd *cmd, char **env);

/********** EXEX UTILS ***********/ 
void		free_tab(char **tab);

void		print_lava_intro(void);

#endif
