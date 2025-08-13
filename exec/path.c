
#include "../include/minishell.h"

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Searches through PATH if command exists  <3
<3 Helper function used in get_path() below <3 
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	*find_path(char **dir, char *cmd)
{
	int		i;
	char	*tmp;
	char	*final_path;

	i = 0;
	while (dir[i]) // Checks each directory in PATH <3
	{
		tmp = ft_strjoin(dir[i], "/"); // Builds full path -> directory + "/" + command <3
		final_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(final_path, F_OK | X_OK) == 0) // Checks if file exists and is executable <3
			return (final_path);
		free(final_path);
		i++;
	}
	return (NULL);
}

/*
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
<3 Finds the full path to a command <3
<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3
*/
char	*get_path(char *cmd, char **env)
{
	int i;
	char *path;
	char **dir;
	char *final_path;

	i = 0;
		if (ft_strchr(cmd, '/') || cmd[0] == '.')
		{
				if (access(cmd, F_OK) == 0)
				{
						DIR *d = opendir(cmd);
						if (d)
						{
								closedir(d);
								return (ft_strdup(cmd));
						}
						return (ft_strdup(cmd));
				}
				return (NULL);
		}
	while (env[i] && ft_strncmp(env[i], "PATH=", 5)) // Finds PATH env var <3
		i++;
	if (!env[i])
		return (NULL);
	path = env[i] + 5; // Skips "PATH=" <3
	dir = ft_split(path, ':'); // Splits PATH by ':' <3
	final_path = find_path(dir, cmd); // Searches through directories <3
	free_tab(dir);
	return (final_path);
}
