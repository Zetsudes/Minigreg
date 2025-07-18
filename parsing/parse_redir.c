#include "../include/minishell.h"

static int	handle_post_redir_err(char *tok)
{
	if (!tok || is_token_operator(tok))
		return (print_syntax_error(tok));
	return (0);
}

int	process_redirs(t_cmd *cmd, char **tk, int *i)
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
int     set_redirection(t_cmd *cmd, char *op, char *file)
{
        if (!ft_strcmp(op, "<"))
        {
                int fd = open(file, O_RDONLY);
                if (fd < 0)
                        cmd->redir_error = (perror(file), 1);
                else
                        close(fd);
                free(cmd->infile);
                cmd->infile = ft_strdup(file);
                cmd->heredoc = 0;
        }
        else if (!ft_strcmp(op, "<<"))
        {
                free(cmd->infile);
                cmd->heredoc = 1;
                cmd->infile = ft_strdup(file);
        }
        else if (!ft_strcmp(op, ">"))
        {
                int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                        cmd->redir_error = (perror(file), 1);
                else
                        close(fd);
                free(cmd->outfile);
                cmd->outfile = ft_strdup(file);
                cmd->append = 0;
        }
        else if (!ft_strcmp(op, ">>"))
        {
                int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0)
                        cmd->redir_error = (perror(file), 1);
                else
                        close(fd);
                free(cmd->outfile);
                cmd->append = 1;
                cmd->outfile = ft_strdup(file);
        }
        if ((op[0] == '<' && !cmd->infile)
                || (op[0] == '>' && !cmd->outfile))
                return (0);
        return (1);
}
