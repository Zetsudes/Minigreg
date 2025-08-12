/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:17:48 by marvin            #+#    #+#             */
/*   Updated: 2025/07/21 08:17:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>

static int	handle_post_redir_err(char *tok)
{
	if (!tok || is_token_operator(tok))
		return (print_syntax_error(tok));
	return (0);
}

int	process_redirs(t_cmd *cmd, char **tk, int *i)
{
	char	*op;
	char	*file;

	while (tk[*i] && ft_strcmp(tk[*i], "|") && ft_strcmp(tk[*i], ";"))
	{
		op = tk[*i];
		file = tk[*i + 1];
		if (is_token_operator(op) && ft_strcmp(op, "|"))
		{
			if (handle_post_redir_err(file))
				return (1);
			if (!set_redirection(cmd, op, file))
				return (1);
			*i += 2;
		}
		else
			(*i)++;
	}
	return (0);
}
