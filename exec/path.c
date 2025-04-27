
# include "../include/minishell.h"

char **copy_env(char **envp)
{
    int len;
	int i;
	char	**env;

	i = 0;
	len = 0;
    while (envp[len])
        len++;
    env = malloc(sizeof(char *) * (len + 1));
    if (!env)
        return (NULL);

    while (i < len)
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
    env[len] = NULL;
    return (env);
}

char	*find_path(char **dir, char *cmd)
{
    int i;
    char	*tmp;
    char	*final_path;

	i = 0;
    while (dir[i])
    {
        tmp = ft_strjoin(dir[i], "/");
        final_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(final_path, F_OK | X_OK) == 0)
            return (final_path);
        free(final_path);
        i++;
    }
    return (NULL);
}

char	*get_path(char *cmd, char **env)
{
    int i;
    char *path;
    char **dir;
    char *final_path;

	i = 0;
    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
    {
        if (access(cmd, F_OK | X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    while (env[i] && ft_strncmp(env[i], "PATH=", 5))
        i++;
    if (!env[i])
        return (NULL);
    path = env[i] + 5;
    dir = ft_split(path, ':');
    final_path = find_path(dir, cmd);
    free_tab(dir);
    return (final_path);
}