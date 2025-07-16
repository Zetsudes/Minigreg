#include "../include/minishell.h"

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