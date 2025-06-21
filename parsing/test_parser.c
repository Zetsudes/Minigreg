// In test_parser.c
#include "parser.h" // This should give access to tokenize_line and parse_tokens
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int test_count = 0;
int pass_count = 0;

void assert_string_equals(const char* s1, const char* s2, const char* test_name) {
    test_count++;
    if (s1 == NULL && s2 == NULL) {
        printf("[OK] %s\n", test_name);
        pass_count++;
        return;
    }
    if (s1 == NULL || s2 == NULL || strcmp(s1, s2) != 0) {
        printf("[FAIL] %s: Expected '%s', Got '%s'\n", test_name, s2, s1);
    } else {
        printf("[OK] %s\n", test_name);
        pass_count++;
    }
}

void assert_null(void* ptr, const char* test_name) {
    test_count++;
    if (ptr == NULL) {
        printf("[OK] %s\n", test_name);
        pass_count++;
    } else {
        printf("[FAIL] %s: Expected NULL, Got %p\n", test_name, ptr);
    }
}

void assert_not_null(void* ptr, const char* test_name) {
    test_count++;
    if (ptr != NULL) {
        printf("[OK] %s\n", test_name);
        pass_count++;
    } else {
        printf("[FAIL] %s: Expected not NULL, Got NULL\n", test_name);
    }
}

void assert_int_equals(int i1, int i2, const char* test_name) {
    test_count++;
    if (i1 == i2) {
        printf("[OK] %s\n", test_name);
        pass_count++;
    } else {
        printf("[FAIL] %s: Expected %d, Got %d\n", test_name, i2, i1);
    }
}

// Helper to free token array
void free_tokens(char **tokens) {
    if (!tokens) return;
    for (int i = 0; tokens[i]; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

// Helper to free command list
void free_test_cmd_list(t_cmd *cmd) {
     t_cmd *tmp;
     while (cmd != NULL) {
         tmp = cmd->next;
         if (cmd->args) {
             for (int k = 0; cmd->args[k]; k++) {
                 free(cmd->args[k]);
             }
             free(cmd->args);
         }
         free(cmd->infile);
         free(cmd->outfile);
         free(cmd);
         cmd = tmp;
     }
}

// Forward declarations for test functions
void test_tokenize_simple_command();
void test_tokenize_operators();
void test_tokenize_single_quotes();
void test_tokenize_double_quotes();
void test_tokenize_escaped_double_quotes();
void test_tokenize_unclosed_quote();
void test_tokenize_dollar_var();
void test_tokenize_dollar_qmark();
void test_tokenize_dollar_literal();

void test_parse_simple_command();
void test_parse_redirection();
void test_parse_pipe();
void test_parse_missing_filename_redirection();
void test_parse_pipe_at_end();
void test_parse_consecutive_pipes();


// --- Tokenizer Tests ---
void test_tokenize_simple_command() {
    char **tokens = tokenize_line("ls -l");
    assert_not_null(tokens, "Tokenizer Simple: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "ls", "Tokenizer Simple: token 0 ('ls')");
        assert_string_equals(tokens[1], "-l", "Tokenizer Simple: token 1 ('-l')");
        assert_null(tokens[2], "Tokenizer Simple: token 2 (NULL)");
    }
    free_tokens(tokens);
}

void test_tokenize_operators() {
    char **tokens = tokenize_line("echo < file | wc >> out");
    assert_not_null(tokens, "Tokenizer Operators: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "echo", "Tokenizer Operators: token 0 ('echo')");
        assert_string_equals(tokens[1], "<", "Tokenizer Operators: token 1 ('<')");
        assert_string_equals(tokens[2], "file", "Tokenizer Operators: token 2 ('file')");
        assert_string_equals(tokens[3], "|", "Tokenizer Operators: token 3 ('|')");
        assert_string_equals(tokens[4], "wc", "Tokenizer Operators: token 4 ('wc')");
        assert_string_equals(tokens[5], ">>", "Tokenizer Operators: token 5 ('>>')");
        assert_string_equals(tokens[6], "out", "Tokenizer Operators: token 6 ('out')");
        assert_null(tokens[7], "Tokenizer Operators: token 7 (NULL)");
    }
    free_tokens(tokens);
}

void test_tokenize_single_quotes() {
    char **tokens = tokenize_line("echo 'hello world'");
    assert_not_null(tokens, "Tokenizer Single Quotes: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "echo", "Tokenizer Single Quotes: token 0 ('echo')");
        assert_string_equals(tokens[1], "hello world", "Tokenizer Single Quotes: token 1 ('hello world')");
        assert_null(tokens[2], "Tokenizer Single Quotes: token 2 (NULL)");
    }
    free_tokens(tokens);
}

void test_tokenize_double_quotes() {
    // As per current implementation, $USER is not expanded by tokenizer
    char **tokens = tokenize_line("echo \"hello $USER\"");
    assert_not_null(tokens, "Tokenizer Double Quotes: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "echo", "Tokenizer Double Quotes: token 0 ('echo')");
        assert_string_equals(tokens[1], "hello $USER", "Tokenizer Double Quotes: token 1 ('hello $USER')");
        assert_null(tokens[2], "Tokenizer Double Quotes: token 2 (NULL)");
    }
    free_tokens(tokens);
}

void test_tokenize_escaped_double_quotes() {
    char **tokens = tokenize_line("echo \"say \\\"hello\\\"\"");
    assert_not_null(tokens, "Tokenizer Escaped Quotes: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "echo", "Tokenizer Escaped Quotes: token 0 ('echo')");
        assert_string_equals(tokens[1], "say \"hello\"", "Tokenizer Escaped Quotes: token 1 ('say \"hello\"')");
        assert_null(tokens[2], "Tokenizer Escaped Quotes: token 2 (NULL)");
    }
    free_tokens(tokens);
}

void test_tokenize_unclosed_quote() {
    // Expecting error message "Erreur : Quote non fermée" from tokenizer.c
    // And tokenize_line should return NULL
    char **tokens = tokenize_line("echo \"hello");
    assert_null(tokens, "Tokenizer Unclosed Quote: tokens are NULL");
    free_tokens(tokens); // Safe to call with NULL
}

void test_tokenize_dollar_var() {
    char **tokens = tokenize_line("echo $VAR/tmp");
    assert_not_null(tokens, "Tokenizer $VAR: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "echo", "Tokenizer $VAR: token 0 ('echo')");
        assert_string_equals(tokens[1], "$VAR", "Tokenizer $VAR: token 1 ('$VAR')");
        assert_string_equals(tokens[2], "/tmp", "Tokenizer $VAR: token 2 ('/tmp')");
        assert_null(tokens[3], "Tokenizer $VAR: token 3 (NULL)");
    }
    free_tokens(tokens);
}

void test_tokenize_dollar_qmark() {
    char **tokens = tokenize_line("echo $?");
    assert_not_null(tokens, "Tokenizer $?: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "echo", "Tokenizer $?: token 0 ('echo')");
        assert_string_equals(tokens[1], "$?", "Tokenizer $?: token 1 ('$?')");
        assert_null(tokens[2], "Tokenizer $?: token 2 (NULL)");
    }
    free_tokens(tokens);
}

void test_tokenize_dollar_literal() {
    char **tokens = tokenize_line("echo $");
    assert_not_null(tokens, "Tokenizer $: tokens not NULL");
    if(tokens) {
        assert_string_equals(tokens[0], "echo", "Tokenizer $: token 0 ('echo')");
        assert_string_equals(tokens[1], "$", "Tokenizer $: token 1 ('$')");
        assert_null(tokens[2], "Tokenizer $: token 2 (NULL)");
    }
    free_tokens(tokens);
}


// --- Parser Tests ---
void test_parse_simple_command() {
    char *line = "ls -l";
    char **tokens = tokenize_line(line);
    assert_not_null(tokens, "Parser Simple: tokenize_line did not return NULL");
    if (!tokens) return;

    t_cmd *cmd_list = parse_tokens(tokens);
    assert_not_null(cmd_list, "Parser Simple: cmd_list not NULL");
    if(cmd_list) {
        assert_not_null(cmd_list->args, "Parser Simple: cmd->args not NULL");
        if (cmd_list->args) {
            assert_string_equals(cmd_list->args[0], "ls", "Parser Simple: cmd->args[0] ('ls')");
            assert_string_equals(cmd_list->args[1], "-l", "Parser Simple: cmd->args[1] ('-l')");
            assert_null(cmd_list->args[2], "Parser Simple: cmd->args[2] (NULL)");
        }
        assert_null(cmd_list->infile, "Parser Simple: infile is NULL");
        assert_null(cmd_list->outfile, "Parser Simple: outfile is NULL");
        assert_int_equals(cmd_list->append, 0, "Parser Simple: append is 0");
        assert_int_equals(cmd_list->heredoc, 0, "Parser Simple: heredoc is 0");
        assert_null(cmd_list->next, "Parser Simple: cmd->next (NULL)");
    }
    free_tokens(tokens);
    free_test_cmd_list(cmd_list);
}

void test_parse_redirection() {
    char *line = "cat < infile > outfile";
    char **tokens = tokenize_line(line);
    assert_not_null(tokens, "Parser Redirection: tokenize_line did not return NULL");
    if (!tokens) return;

    t_cmd *cmd_list = parse_tokens(tokens);
    assert_not_null(cmd_list, "Parser Redirection: cmd_list not NULL");
    if(cmd_list) {
        assert_not_null(cmd_list->args, "Parser Redirection: cmd->args not NULL");
        if (cmd_list->args) {
            assert_string_equals(cmd_list->args[0], "cat", "Parser Redirection: cmd->args[0] ('cat')");
            assert_null(cmd_list->args[1], "Parser Redirection: cmd->args[1] (NULL)");
        }
        assert_string_equals(cmd_list->infile, "infile", "Parser Redirection: infile ('infile')");
        assert_string_equals(cmd_list->outfile, "outfile", "Parser Redirection: outfile ('outfile')");
        assert_int_equals(cmd_list->append, 0, "Parser Redirection: append is 0");
        assert_int_equals(cmd_list->heredoc, 0, "Parser Redirection: heredoc is 0");
        assert_null(cmd_list->next, "Parser Redirection: cmd->next (NULL)");
    }
    free_tokens(tokens);
    free_test_cmd_list(cmd_list);
}

void test_parse_pipe() {
    char *line = "ls | wc";
    char **tokens = tokenize_line(line);
    assert_not_null(tokens, "Parser Pipe: tokenize_line did not return NULL");
    if (!tokens) return;
    
    t_cmd *cmd_list = parse_tokens(tokens);
    assert_not_null(cmd_list, "Parser Pipe: cmd_list not NULL");

    if(cmd_list) { // First command: ls
        assert_not_null(cmd_list->args, "Parser Pipe: cmd1->args not NULL");
        if(cmd_list->args) assert_string_equals(cmd_list->args[0], "ls", "Parser Pipe: cmd1->args[0] ('ls')");
        assert_null(cmd_list->infile, "Parser Pipe: cmd1->infile is NULL");
        assert_null(cmd_list->outfile, "Parser Pipe: cmd1->outfile is NULL");
        assert_not_null(cmd_list->next, "Parser Pipe: cmd1->next not NULL");

        if(cmd_list->next) { // Second command: wc
            t_cmd *cmd2 = cmd_list->next;
            assert_not_null(cmd2->args, "Parser Pipe: cmd2->args not NULL");
            if(cmd2->args) assert_string_equals(cmd2->args[0], "wc", "Parser Pipe: cmd2->args[0] ('wc')");
            assert_null(cmd2->infile, "Parser Pipe: cmd2->infile is NULL");
            assert_null(cmd2->outfile, "Parser Pipe: cmd2->outfile is NULL");
            assert_null(cmd2->next, "Parser Pipe: cmd2->next (NULL)");
        }
    }
    free_tokens(tokens);
    free_test_cmd_list(cmd_list);
}

void test_parse_missing_filename_redirection() {
    char *line = "cat <";
    char **tokens = tokenize_line(line);
    // Tokenizer should be fine with this.
    assert_not_null(tokens, "Parser Missing Filename: tokenize_line not NULL");
    if (!tokens) return;

    // parse_tokens should return NULL and print an error
    // "minishell: syntax error near unexpected token `newline'"
    t_cmd *cmd_list = parse_tokens(tokens);
    assert_null(cmd_list, "Parser Missing Filename: cmd_list is NULL");
    
    free_tokens(tokens);
    free_test_cmd_list(cmd_list); // Safe to call with NULL
}

void test_parse_pipe_at_end() {
    char *line = "ls |";
    char **tokens = tokenize_line(line);
    assert_not_null(tokens, "Parser Pipe At End: tokenize_line not NULL");
    if (!tokens) return;

    // parse_tokens should return NULL and print an error
    // "minishell: syntax error near unexpected token `|'" (or newline/specifics)
    t_cmd *cmd_list = parse_tokens(tokens);
    assert_null(cmd_list, "Parser Pipe At End: cmd_list is NULL");

    free_tokens(tokens);
    free_test_cmd_list(cmd_list);
}

void test_parse_consecutive_pipes() {
    char *line = "ls || wc"; // Tokenizer might produce "ls", "|", "|", "wc"
    char **tokens = tokenize_line(line);
    assert_not_null(tokens, "Parser Consecutive Pipes: tokenize_line not NULL");
    if (!tokens) return;
    
    // parse_tokens should return NULL and print an error
    // "minishell: syntax error near unexpected token `|'"
    t_cmd *cmd_list = parse_tokens(tokens);
    assert_null(cmd_list, "Parser Consecutive Pipes: cmd_list is NULL");

    free_tokens(tokens);
    free_test_cmd_list(cmd_list);
}


int main() {
    printf("--- Running Tokenizer Tests ---\n");
    test_tokenize_simple_command();
    test_tokenize_operators();
    test_tokenize_single_quotes();
    test_tokenize_double_quotes();
    test_tokenize_escaped_double_quotes();
    test_tokenize_unclosed_quote();
    test_tokenize_dollar_var();
    test_tokenize_dollar_qmark();
    test_tokenize_dollar_literal();
    // Add more calls as new tokenizer tests are written

    printf("\n--- Running Parser Tests ---\n");
    test_parse_simple_command();
    test_parse_redirection();
    test_parse_pipe();
    test_parse_missing_filename_redirection();
    test_parse_pipe_at_end();
    test_parse_consecutive_pipes();
    // Add more calls as new parser tests are written

    printf("\n--- Test Summary ---\n");
    printf("Total Tests: %d\n", test_count);
    printf("Passed: %d\n", pass_count);
    printf("Failed: %d\n", test_count - pass_count);
    return (test_count - pass_count); // Return number of failures
}

// Ensure parser.h provides:
// typedef struct s_cmd { ... } t_cmd;
// char **tokenize_line(char *line);
// t_cmd *parse_tokens(char **tokens);
// And that the t_cmd definition is available.
// Also, if free_cmd_list is static in parsing.c, then free_test_cmd_list is needed.
// The libft functions like ft_strcmp, ft_strdup must be linked.
// The is_operator, ft_substr from tokenizer.c must be available if not static.
// count_args, copy_args, init_cmd from parsing.c must be available if not static.
// For now, these tests assume they can call tokenize_line and parse_tokens,
// and that the necessary struct definitions are exposed.
// The Makefile will be crucial for linking these correctly.
