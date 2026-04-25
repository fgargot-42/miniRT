/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:47:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/23 17:49:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

void	print_parse_error(char *message, char *element, int line_nb)
{
	ft_putstr_fd("Error\nLine ", 2);
	ft_putnbr_fd(line_nb, 2);
	ft_putstr_fd(": ", 2);
	if (message)
		ft_putstr_fd(message, 2);
	if (element)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(element, 2);
	}
	if (message || element)
		ft_putstr_fd("\n", 2);
}

void	clear_gnl(int fd, char *line)
{
	close(fd);
	if (line)
		free(line);
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
}
