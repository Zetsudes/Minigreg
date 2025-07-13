/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkantzer <lkantzer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:10:00 by lkantzer          #+#    #+#             */
/*   Updated: 2025/06/28 15:55:00 by lkantzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>

/* -------------------------------------------------------------------------- */

/*
**  utils : impression d’un message d’erreur « unexpected token »
*/
static int	print_syntax_error(const char *tok)
{
	const char	*prefix =
		"minishell: syntax error near unexpected token `";
	const char	*suffix = "'\n";

	if (!tok)
		tok = "newline";
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, tok, ft_strlen(tok));
	write(STDERR_FILENO, suffix, 2);
	return (1);
}

/* -------------------------------------------------------------------------- */

static int	handle_pipe_errors(char **tk, int i, t_cmd *cmd)
{
	if (!cmd->args[0] && !cmd->infile && !cmd->outfile)
		return (print_syntax_error("|"));
	if (!tk[i] || !ft_strcmp(tk[i], "|"))
		return (print_syntax_error("|"));
	return (0);
}

static int	handle_post_redir_err(char *tok)
{
	if (!tok || is_token_operator(tok))
		return (print_syntax_error(tok));
	return (0);
}

/* -------------------------------------------------------------------------- */

static int	process_redirs(t_cmd *cmd, char **tk, int *i)
{
	char	*op;
	char	*file;

	while (tk[*i] && ft_strcmp(tk[*i], "|") && ft_strcmp(tk[*i], ";"))
	{
		op = tk[*i];
		file = tk[*i + 1];
		if (is_token_operator(op) && ft_strcmp(op, "|"))
		{
			if (handle_post_redir_err(file))
				return (1);
			if (!set_redirection(cmd, op, file))
				return (1);
			*i += 2;
		}
		else
			(*i)++;
	}
	return (0);
}

/* -------------------------------------------------------------------------- */

int	process_segment(t_cmd **head, t_cmd **cur, char **tk, int *i, t_env *env)
{
	t_cmd	*new;

	new = init_cmd();
	if (!new)
	{
		free_cmd_list(*head);
		return (1);
	}
	if (!*head)
		*head = new;
	else
		(*cur)->next = new;
	*cur = new;
	new->args = copy_args(tk, i, env);
	if (!new->args)
	{
		free_cmd_list(*head);
		return (1);
	}
	if (process_redirs(new, tk, i))
	{
		free_cmd_list(*head);
		return (1);
	}
	return (0);
}

/* -------------------------------------------------------------------------- */

t_cmd	*parse_tokens(char **tk, t_env *env)
{
	t_cmd	*head;
	t_cmd	*cur;
	int		i;

	head = NULL;
	cur = NULL;
	i = 0;
	if (!tk || !tk[0])
		return (NULL);
	if (!ft_strcmp(tk[0], "|"))
		return (print_syntax_error("|"), NULL);
	while (tk[i])
	{
		if (process_segment(&head, &cur, tk, &i, env))
			return (NULL);
		if (tk[i] && !ft_strcmp(tk[i], "|"))
		{
			i++;
			if (handle_pipe_errors(tk, i, cur))
				return (free_cmd_list(head), NULL);
		}
		else if (tk[i] && !ft_strcmp(tk[i], ";"))
		{
            cur->cmd_separator = 1;
			i++;
			if (!tk[i] || !ft_strcmp(tk[i], ";") || !ft_strcmp(tk[i], "|"))
				return (print_syntax_error(tk[i]), free_cmd_list(head), NULL);
		}
		else if (tk[i])
			return (print_syntax_error(tk[i]), free_cmd_list(head), NULL);
	}
	return (head);
}

/* -------------------------------------------------------------------------- */

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = next;
	}
}
