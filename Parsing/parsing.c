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

// static char	**copy_args(char **tokens, int *i) faut copier les arguments du token dans la structure

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

// t_cmd	*parse_tokens(char **tokens)