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

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
}			t_cmd;

char		**split_args(char *line);

/********** PATH ***********/ 
char		*find_path(char **dir, char *cmd);
char		*get_path(char *cmd, char **env);

/********** EXEC ***********/ 
void		execute_command(t_cmd *cmd, char **env);

/********** EXEX UTILS ***********/ 
void		free_tab(char **tab);

void		print_lava_intro(void);

#endif
