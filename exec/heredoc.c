/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:35:16 by marvin            #+#    #+#             */
/*   Updated: 2025/08/22 17:35:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	hd_is_quoted(const char *s)
{
	while (s && *s)
	{
		if (*s == '\'' || *s == '"')
			return (1);
		s++;
	}
	return (0);
}

static char	*hd_strip_quotes(const char *s)
{
	size_t	i;
	size_t	k;
	char	*out;

	if (!s)
		return (ft_strdup(""));
	out = malloc(ft_strlen(s) + 1);
	if (!out)
		return (NULL);
	i = 0;
	k = 0;
	while (s[i])
	{
		if (s[i] != '\'' && s[i] != '"')
			out[k++] = s[i];
		i++;
	}
	out[k] = '\0';
	return (out);
}

static void	hd_rstrip_nl(char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	if (len && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

static int	hd_write_line(int fd, char *line, int expand, t_env *env)
{
	char	*tmp;

	if (!expand)
	{
		ft_putendl_fd(line, fd);
		return (1);
	}
	tmp = expand_var(line, env);
	if (!tmp)
		tmp = ft_strdup("");
	if (!tmp)
		return (0);
	ft_putendl_fd(tmp, fd);
	free(tmp);
	return (1);
}

int	heredoc_to_fd(const char *lim_raw, t_env *env)
{
	int		pfd[2];
	char	*lim;
	char	*line;
	int		expand;

	expand = !hd_is_quoted(lim_raw);
	lim = hd_strip_quotes(lim_raw);
	if (!lim || pipe(pfd) == -1)
		return (free(lim), -1);
	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		hd_rstrip_nl(line);
		if (ft_strcmp(line, lim) == 0)
			return (free(line), free(lim), close(pfd[1]), pfd[0]);
		if (!hd_write_line(pfd[1], line, expand, env))
			return (free(line), free(lim), close(pfd[0]), close(pfd[1]), -1);
		free(line);
	}
	free(lim);
	close(pfd[1]);
	return (pfd[0]);
}
