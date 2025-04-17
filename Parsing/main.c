#include <stdio.h>
#include "parser.h"

int main() {
    char *line = "echo hello world > output.txt | grep foo < input.txt";
    
    char **tokens = tokenize_line(line);

    // Afficher tous les tokens
    for (int i = 0; tokens[i] != NULL; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i]);
        free(tokens[i]);  // N'oublie pas de libérer la mémoire allouée pour chaque token
    }

    free(tokens);  // Libérer le tableau de tokens
    return 0;
}
