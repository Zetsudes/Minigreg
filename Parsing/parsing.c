#include "../include/minishell.h"
#include <stdio.h>

static int	handle_pipe_errors(char **tokens, int i, t_cmd *cmd)
{
	if (!cmd->args[0] && !cmd->infile && !cmd->outfile)
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	if (!tokens[i] || !ft_strcmp(tokens[i], "|"))
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

static int	handle_post_redirection_errors(char *token)
{
	if (!token || is_token_operator(token))
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `%s'\n",
			token ? token : "newline");
		return (1);
	}
	return (0);
}

static int	process_redirections(t_cmd *cmd, char **tokens, int *i)
{
	char	*op;
	char	*file;

	while (tokens[*i] && ft_strcmp(tokens[*i], "|"))
	{
		op = tokens[*i];
		file = tokens[*i + 1];
		if (is_token_operator(op) && ft_strcmp(op, "|"))
		{
			if (handle_post_redirection_errors(file))
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

int	process_command_segment(t_cmd **head, t_cmd **curr, char **tokens, int *i, t_env *env)
{
	t_cmd	*new;

	new = init_cmd();
	if (!new)
		return (free_cmd_list(*head), 1);
	if (!*head)
		*head = new;
	else
		(*curr)->next = new;
	*curr = new;
	new->args = copy_args(tokens, i, env); // version avec env pour expansion
	if (!new->args)
		return (free_cmd_list(*head), 1);

	if (process_redirections(new, tokens, i))
		return (free_cmd_list(*head), 1);

	return (0);
}

t_cmd *parse_tokens(char **tokens, t_env *env)
{
	t_cmd	*head;
	t_cmd	*curr;
	int		i;

	head = NULL;
	curr = NULL;
	i = 0;
	if (!tokens || !tokens[0])
		return (NULL);
	if (!ft_strcmp(tokens[0], "|"))
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `|'\n");
		return (NULL);
	}
	while (tokens[i])
	{
		if (process_command_segment(&head, &curr, tokens, &i, env))
			return (NULL);
		if (tokens[i] && !ft_strcmp(tokens[i], "|"))
		{
			i++;
			if (handle_pipe_errors(tokens, i, curr))
				return (free_cmd_list(head), NULL);
		}
		else if (tokens[i])
		{
			fprintf(stderr,
				"minishell: syntax error near unexpected token `%s'\n",
				tokens[i]);
			return (free_cmd_list(head), NULL);
		}
	}
	return (head);
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list;
		cmd_list = cmd_list->next;
		free(tmp->args);
		free(tmp->infile);
		free(tmp->outfile);
		free(tmp);
	}
}