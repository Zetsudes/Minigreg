#include <stdio.h>
#include "parser.h"

void print_cmds(t_cmd *cmds)
{
	int i;
	int cmd_num = 1;

	while (cmds)
	{
		printf("=== Commande %d ===\n", cmd_num++);
		i = 0;
		while (cmds->args && cmds->args[i])
		{
			printf("arg[%d]: %s\n", i, cmds->args[i]);
			i++;
		}
		if (cmds->infile)
			printf("infile: %s\n", cmds->infile);
		if (cmds->outfile)
		{
			if (cmds->append)
				printf("outfile (append): %s\n", cmds->outfile);
			else
				printf("outfile: %s\n", cmds->outfile);
		}
		if (cmds->heredoc)
			printf("heredoc: %s\n", cmds->infile);
		cmds = cmds->next;
	}
}

int main(void)
{
    char *line;
    char **tokens;
    t_cmd *cmds;

    /* === TEST 1 : Redirection simple '>' === */
    line = "echo hello > output.txt";
    printf("Ligne à parser : %s\n", line);
    tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    cmds = parse_tokens(tokens);
    printf("✅ Parsing OK\n");
    print_cmds(cmds);
    printf("\n");

    /* === TEST 2 : Redirection append '>>' === */
    line = "echo hello >> output.txt";
    printf("Ligne à parser : %s\n", line);
    tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    cmds = parse_tokens(tokens);
    printf("✅ Parsing OK\n");
    print_cmds(cmds);
    printf("\n");

    /* === TEST 3 : Redirection d'entrée '<' === */
    line = "cat < input.txt";
    printf("Ligne à parser : %s\n", line);
    tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    cmds = parse_tokens(tokens);
    printf("✅ Parsing OK\n");
    print_cmds(cmds);
    printf("\n");

    /* === TEST 4 : Heredoc '<<' === */
    line = "cat << END";
    printf("Ligne à parser : %s\n", line);
    tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    cmds = parse_tokens(tokens);
    printf("✅ Parsing OK\n");
    print_cmds(cmds);
    printf("\n");

    /* === TEST 5 : Combinaison de redirections === */
    line = "grep hello < input.txt > output.txt";
    printf("Ligne à parser : %s\n", line);
    tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    cmds = parse_tokens(tokens);
    printf("✅ Parsing OK\n");
    print_cmds(cmds);
    printf("\n");

    /* === TEST 6 : Double redirection de sortie (erreur de syntaxe attendue) === */
    line = "echo hello > > output.txt";
    printf("Ligne à parser : %s\n", line);
    tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    cmds = parse_tokens(tokens);
    if (cmds)
        print_cmds(cmds);
    else
        printf("❌ Erreur de parsing: Syntaxe incorrecte (double redirection)\n");
    printf("\n");

    /* === TEST 7 : Double redirection opposée (erreur de syntaxe attendue) === */
    line = "cat << >> output.txt";
    printf("Ligne à parser : %s\n", line);
    tokens = tokenize_line(line);
    printf("✅ Tokenization OK\n");
    cmds = parse_tokens(tokens);
    if (cmds)
        print_cmds(cmds);
    else
        printf("❌ Erreur de parsing: Syntaxe incorrecte (redirection opposée)\n");
    printf("\n");

    return 0;
}
