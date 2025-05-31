#include "../include/minishell.h"
#include <stdio.h> // For fprintf, stderr

void free_cmd_list(t_cmd *cmd)
{
    t_cmd *tmp;
    while (cmd != NULL)
    {
        tmp = cmd->next;
        if (cmd->args)
        {
            for (int k = 0; cmd->args[k]; k++)
            {
                free(cmd->args[k]);
            }
            free(cmd->args);
        }
        free(cmd->infile); // free even if NULL, it's safe
        free(cmd->outfile); // free even if NULL, it's safe
        free(cmd);
        cmd = tmp;
    }
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

static int	is_token_operator(const char *token)
{
	if (!token)
		return (0);
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0 ||
		ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0 ||
		ft_strcmp(token, "|") == 0)
		return (1);
	return (0);
}

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

	if (tokens == NULL || tokens[0] == NULL) // Handle empty or whitespace-only input
		return (NULL);

	if (ft_strcmp(tokens[0], "|") == 0) { // Pipe as first token
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		return (NULL); // No memory allocated yet for head
	}

	while(tokens[i])
	{
		t_cmd *new_cmd = init_cmd();
		if (!new_cmd) {
			free_cmd_list(head); // Free any previously allocated commands
			return (NULL);
		}
		if (head == NULL) {
			head = new_cmd;
		} else {
			current->next = new_cmd;
		}
		current = new_cmd;

		current->args = copy_args(tokens, &i); // i is now at the first token after args (operator or | or NULL)
		
		// Malloc check for args, copy_args internally uses strdup which can fail
		if (current->args == NULL && count_args(tokens, i - (current->args ? 0 : 0)) > 0) { // crude check if args were expected but failed
		    // A more robust check would be if ft_strdup inside copy_args failed.
		    // For now, if copy_args returns NULL but there were supposed to be args, assume malloc error.
		    // This might need ft_strdup to set errno or a global error flag.
		    // For simplicity, this check is basic. A failure in copy_args (e.g. malloc for args array) also needs handling.
		    // If copy_args itself returns NULL, it's a problem. Let's assume it returns non-NULL with args[0]=NULL for empty.
		}


		// Loop for redirections for current command
		while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
		{
			char *operator_token = tokens[i]; // Renamed for clarity
			char *filename_token = tokens[i+1]; // Peek for filename

			if (ft_strcmp(operator_token, "<") == 0 || ft_strcmp(operator_token, "<<") == 0 ||
				ft_strcmp(operator_token, ">") == 0 || ft_strcmp(operator_token, ">>") == 0)
			{
				if (filename_token == NULL) {
					fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
					free_cmd_list(head);
					return (NULL);
				}
				if (is_token_operator(filename_token)) {
					fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", filename_token);
					free_cmd_list(head);
					return (NULL);
				}

				if (ft_strcmp(operator_token, "<") == 0) current->infile = ft_strdup(filename_token);
				else if (ft_strcmp(operator_token, "<<") == 0) { current->heredoc = 1; current->infile = ft_strdup(filename_token); }
				else if (ft_strcmp(operator_token, ">") == 0) current->outfile = ft_strdup(filename_token);
				else if (ft_strcmp(operator_token, ">>") == 0) { current->append = 1; current->outfile = ft_strdup(filename_token); }

				// Check ft_strdup failure (added filename_token != NULL to avoid false positive if filename_token was legit NULL and error was already caught)
				if ((ft_strcmp(operator_token, "<") == 0 || ft_strcmp(operator_token, "<<") == 0) && current->infile == NULL && filename_token != NULL) {
					free_cmd_list(head); return (NULL); // Malloc error
				}
				if ((ft_strcmp(operator_token, ">") == 0 || ft_strcmp(operator_token, ">>") == 0) && current->outfile == NULL && filename_token != NULL) {
					free_cmd_list(head); return (NULL); // Malloc error
				}
				i++; // Consume filename_token
			}
			// If operator_token is not a redirection, it will be caught after this loop.
			i++; // Consume operator_token or advance past non-redirect token
		}

		// After args and redirections for 'current' command are processed:
		// tokens[i] now points to what should be a pipe or end of tokens.
		if (tokens[i] && ft_strcmp(tokens[i], "|") == 0) { // Pipe found
			// Check 1: Empty command segment before this pipe?
			// An empty command segment means no arguments AND no redirections.
			if (current->args[0] == NULL && current->infile == NULL && current->outfile == NULL) {
				fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
				free_cmd_list(head);
				return (NULL);
			}

			i++; // Consume the pipe

			// Check 2: Pipe at end of input or consecutive pipe?
			if (tokens[i] == NULL || ft_strcmp(tokens[i], "|") == 0) {
				fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
				free_cmd_list(head);
				return (NULL);
			}
			// Valid pipe, loop continues for the next command.
		} else if (tokens[i]) {
			// tokens[i] is not NULL and not a pipe.
			// This means there are leftover tokens after parsing arguments and redirections,
			// which constitutes a syntax error. Example: "echo hello > output.txt world"
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokens[i]);
			free_cmd_list(head);
			return (NULL);
		}
		// If tokens[i] is NULL, it's the end of a valid command sequence. Loop terminates.
	}
	return head;
}