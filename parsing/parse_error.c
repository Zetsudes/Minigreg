/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:16:59 by marvin            #+#    #+#             */
/*   Updated: 2025/07/21 08:16:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	print_syntax_error(const char *tok)
{
	const char	*prefix =
		"minishell: syntax error near unexpected token `";
	const char	*suffix = "'\n";

	if (!tok)
		tok = "newline";
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, tok, ft_strlen(tok));
	write(STDERR_FILENO, suffix, 2);
	return (1);
}

int	handle_pipe_errors(char **tk, int i, t_cmd *cmd)
{
	if (!cmd->args[0] && !cmd->infile && !cmd->outfile)
		return (print_syntax_error("|"));
	if (!tk[i] || !ft_strcmp(tk[i], "|"))
		return (print_syntax_error("|"));
	return (0);
}
