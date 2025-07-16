/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:47:37 by zamohame          #+#    #+#             */
/*   Updated: 2025/06/21 16:16:34 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define RED "\001\033[1;31m\002"
# define ORANGE "\001\033[38;5;208m\002"
# define YELLOW "\033[1;33m\002"
# define RESET "\001\033[0m\002"

# include "../greg_lib/libft.h"
# include "builtins.h"
# include "env.h"
# include "exec.h"
# include "pipe.h"
# include "signals.h"
# include "lexer.h"
# include "parsing.h"

void	init_signals(void);

#endif
