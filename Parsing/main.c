#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

void	print_cmds(t_cmd *cmds)
{
	int		i;
	t_cmd	*tmp = cmds;
	int		cmd_num = 1;

	while (tmp)
	{
		printf("=== Commande %d ===\n", cmd_num++);
		i = 0;
		while (tmp->args && tmp->args[i])
		{
			printf("arg[%d]: %s\n", i, tmp->args[i]);
			i++;
		}
		if (tmp->infile)
			printf("infile: %s\n", tmp->infile);
		if (tmp->outfile)
			printf("outfile: %s\n", tmp->outfile);
		if (tmp->append)
			printf("append: true\n");
		if (tmp->heredoc)
			printf("heredoc: true\n");
		tmp = tmp->next;
	}
}

int main(void)
{
    char *line = "echo \"hello world\" > output.txt | grep foo < input.txt";
    printf("Ligne à parser : %s\n", line);
    char **tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    t_cmd *cmds = parse_tokens(tokens);
    printf("✅ Parsing OK\n");
    print_cmds(cmds);
    return 0;
}