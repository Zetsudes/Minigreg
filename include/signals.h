/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zamohame <zamohame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 08:40:56 by zamohame          #+#    #+#             */
/*   Updated: 2025/08/14 10:31:37 by zamohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

void	handle_sigint(int signum);
void	init_signals(void);
void	handle_sigquit(int signum);
int     get_exit_status(int status);

#endif
