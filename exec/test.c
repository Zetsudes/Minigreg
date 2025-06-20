#include "../include/minishell.h"

void print_cmd(t_cmd *cmd)
{
    for (int i = 0; cmd->args && cmd->args[i]; i++)
        printf("  arg[%d]: '%s'\n", i, cmd->args[i]);
    printf("  infile: '%s'\n", cmd->infile ? cmd->infile : "NULL");
    printf("  outfile: '%s'\n", cmd->outfile ? cmd->outfile : "NULL");
    printf("  append: %d\n", cmd->append);
    printf("  heredoc: %d\n", cmd->heredoc);
	printf("\n");
}

int main(int argc, char **argv, char **envp) 
{
    char    *line;
    char    **tokens;
    t_cmd   *cmds;
    t_env   *env;
    
    (void)argc;
    (void)argv;
    env = init_list(envp);
    print_lava_intro();
    
    while (1)
    {
        line = readline(ORANGE "🔥 minihell$ " RESET);
        if (!line)
            break;
            
        if (*line) // Only process non-empty lines
        {
            add_history(line);
            tokens = tokenize_line(line);
            cmds = parse_tokens(tokens, env);
            
            if (cmds)
            {
                // Debug print (remove in final version)
                t_cmd *curr = cmds;
                int idx = 0;
                while (curr)
                {
                    printf("Command #%d:\n", idx++);
                    print_cmd(curr);
                    curr = curr->next;
                }
                
                // Execute the pipeline - pass &env instead of env_to_array(env)
                execute_commands(cmds, &env);
                free_cmd_list(cmds);
            }
            else
            {
                ft_putstr_fd("Parser returned NULL (syntax error?)\n", 2);
            }
            
            free_tab(tokens);
        }
        free(line);
    }
    
    free_env_list(env);
    printf(RED "\n🔥 Fading back into the flames...\n\n" RESET);
    return (0);
}

