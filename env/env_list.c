
#include "../include/minishell.h"

void add_node(t_env **first, t_env **last, t_env *new_node)
{
    if (!*first)
    {
        *first = new_node;
        *last = new_node;
    }
    else
    {
        (*last)->next = new_node;
        *last = new_node;
    }
}

t_env *handle_var(char *env_str, t_env *first)
{
    char    **split;
    t_env   *new_node;

    split = ft_split(env_str, '=');
    if (!split)
    {
        free_env_list(first);
        return (NULL);
    }
    new_node = create_node(split[0], split[1]);
    free_tab(split);
    if (!new_node)
    {
        free_env_list(first);
        return (NULL);
    }
    return (new_node);
}

t_env *init_list(char **envp)
{
    int     i;
    t_env   *first;
    t_env   *last;
    t_env   *new_node;

    first = NULL;
    last = NULL;
    i = 0;
    while (envp[i])
    {
        new_node = handle_var(envp[i], first);
        if (!new_node)
            return (NULL);     
        add_node(&first, &last, new_node);
        i++;
    }
    return (first);
}

t_env *create_node(char *key, char *value)
{
    t_env *node;

    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = ft_strdup(key);
    if (!node->key)
    {
        free(node);
        return (NULL);
    }
    if (value)
        node->value = ft_strdup(value);
    else
        node->value = ft_strdup(""); 
    if (!node->value)
    {
        free(node->key);
        free(node);
        return (NULL);
    }
    node->next = NULL;
    return (node);
}

void free_env_list(t_env *head)
{
    t_env *tmp;
    
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}