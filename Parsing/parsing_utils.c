#include "../include/minishell.h"

int	is_token_operator(const char *token)
{
	if (!token)
		return (0);
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>")
		|| !ft_strcmp(token, "|"));
}

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
	if ((op[0] == '<' && !cmd->infile) || (op[0] == '>' && !cmd->outfile))
		return (0);
	return (1);
}

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
	return (cmd);
}

char	**copy_args(char **tokens, int *i)
{
	char	**args;
	int		count;
	int		j;

	count = count_args(tokens, *i);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	j = 0;
	while (tokens[*i] && tokens[*i][0] != '|'
		&& tokens[*i][0] != '<' && tokens[*i][0] != '>')
		args[j++] = ft_strdup(tokens[(*i)++]);
	args[j] = NULL;
	return (args);
}

int	count_args(char **tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i] && tokens[i][0] != '|'
		&& tokens[i][0] != '<' && tokens[i][0] != '>')
	{
		count++;
		i++;
	}
	return (count);
}

int ft_strcmp(const char *s1, const char *s2)
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