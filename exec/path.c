
# include "../include/minishell.h"

char	*get_path(char *cmd, char **envp)
{
	char	*path;
	char	**dir;
	char	*final_path;
	char	*tmp;
	int i;

	i = 0;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
    {
        if (access(cmd, F_OK | X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
	while(envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return(NULL);
	path = envp[i] + 5;
	dir = ft_split(path, ':');
	i = 0;
	while (dir[i])
	{
		tmp = ft_strjoin(dir[i], "/");
		final_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(final_path, F_OK | X_OK) == 0)
		{
			free_tab(dir);
			return (final_path);
		}
		free(final_path);
		i++;
	}
    printf("Checking path: %s\n", final_path);

	free_tab(dir);
	return (NULL);
}

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}