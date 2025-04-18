#include "parser.h"

static int	count_args(char **tokens, int i)
{
	int count = 0;

	while (tokens[i] && tokens[i][0] != '|' && tokens[i][0] != '<' && tokens[i][0] != '>')
	{
		count++;
		i++;
	}
	return count;
}

static char	**copy_args(char **tokens, int *i)
{
	int		count = count_args(tokens, *i);
	char	**args = malloc(sizeof(char *) * (count + 1));
	int		j = 0;

	while (tokens[*i] && tokens[*i][0] != '|' && tokens[*i][0] != '<' && tokens[*i][0] != '>')
	{
		args[j++] = ft_strdup(tokens[*i]);
		(*i)++;
	}
	args[j] = NULL;
	return args;
}

t_cmd *init_cmd()
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return NULL;
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	return cmd;
}

t_cmd	*parse_tokens(char **tokens)
{
	t_cmd *head = NULL;
	t_cmd *current = NULL;
	int i = 0;

	while(tokens[i])
	{
		t_cmd *new_cmd = init_cmd();
		if (!new_cmd)
			return NULL;
		if (head == NULL)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		current->args = copy_args(tokens, &i);
		while (tokens[i] && ft_strncmp(tokens[i], "|", 2) != 0)
		{
			if (!ft_strncmp(tokens[i], "<",2))
				current->infile = ft_strdup(tokens[++i]);
			else if (!ft_strncmp(tokens[i], "<<", 3))
			{
				current->heredoc = 1;
				current->infile = ft_strdup(tokens[++i]);
			}
			else if (!ft_strncmp(tokens[i], ">",2))
				current->outfile = ft_strdup(tokens[++i]);
			else if (!ft_strncmp(tokens[i], ">>", 3))
			{
				current->append = 1;
				current->outfile = ft_strdup(tokens[++i]);
			}
			i++;
		}
		if (tokens[i] && !ft_strncmp(tokens[i], "|", 2))
			i++;
	}
	return head;
}