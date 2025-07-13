/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkantzer <lkantzer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:42:00 by lkantzer          #+#    #+#             */
/*   Updated: 2025/06/28 11:00:00 by lkantzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** is_token_operator:
**   Retourne 1 si le token correspond à un opérateur de redirection ou de pipe,
**   0 sinon.
*/
int	is_token_operator(const char *token)
{
	if (!token)
		return (0);
	return (!ft_strcmp(token, "<")
		|| !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<")
		|| !ft_strcmp(token, ">>")
		|| !ft_strcmp(token, "|")
		|| !ft_strcmp(token, ";"));
}

/*
** set_redirection:
**   Renseigne les champs infile/outfile/heredoc/append du t_cmd selon l’opérateur
**   rencontré. Renvoie 1 en cas de succès, 0 si une allocation a échoué.
*/
int	set_redirection(t_cmd *cmd, char *op, char *file)
{
	if (!ft_strcmp(op, "<"))
		cmd->infile = ft_strdup(file);
	else if (!ft_strcmp(op, "<<"))
	{
		cmd->heredoc = 1;
		cmd->infile = ft_strdup(file);
	}
	else if (!ft_strcmp(op, ">"))
		cmd->outfile = ft_strdup(file);
	else if (!ft_strcmp(op, ">>"))
	{
		cmd->append = 1;
		cmd->outfile = ft_strdup(file);
	}
	if ((op[0] == '<' && !cmd->infile)
		|| (op[0] == '>' && !cmd->outfile))
		return (0);
	return (1);
}

/*
** init_cmd:
**   Alloue et initialise une structure t_cmd vide.
*/
t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	cmd->cmd_separator = 0;
	return (cmd);
}

/*
** copy_args:
**   Copie les arguments normaux (ni opérateurs ni pipes) à partir de tokens[*i]
**   jusqu’au prochain séparateur. Les variables sont étendues via process_token.
**   *i est mis à jour pour pointer sur le token suivant.
*/
char	**copy_args(char **tokens, int *i, t_env *env)
{
	char	**args;
	int		start;
	int		j;

	start = *i;
	while (tokens[*i] && ft_strcmp(tokens[*i], "|")
		&& ft_strcmp(tokens[*i], ";")
		&& !is_token_operator(tokens[*i]))
		(*i)++;
	args = malloc(sizeof(char *) * (*i - start + 1));
	if (!args)
		return (NULL);
	j = 0;
	while (start < *i)
	{
		args[j] = process_token(tokens[start], env);
		if (!args[j])
		{
			while (j-- > 0)
				free(args[j]);
			free(args);
			return (NULL);
		}
		j++;
		start++;
	}
	args[j] = NULL;
	return (args);
}

/*
** count_args:
**   Renvoie le nombre de tokens « argument » (ni opérateurs, ni pipes)
**   à partir de l’index i.
*/
int	count_args(char **tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i] && tokens[i][0] != '|'
		&& tokens[i][0] != ';'
		&& tokens[i][0] != '<' && tokens[i][0] != '>')
	{
		count++;
		i++;
	}
	return (count);
}

/*
** ft_strcmp:
**   Version minimale de strcmp qui gère les pointeurs NULL.
*/
int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (1);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/*
** process_token:
**   Applique l’expansion de variable au token donné. Renvoie une chaîne
**   dupliquée ; chaîne vide en cas d’erreur ou de NULL.
*/
char	*process_token(char *tok, t_env *env)
{
	char	*out;

	if (!tok)
		return (ft_strdup(""));
	out = expand_var(tok, env);
	if (!out)
		out = ft_strdup("");
	return (out);
}
