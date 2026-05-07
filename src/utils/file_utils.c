/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 23:36:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/08 00:39:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

static int	check_file_errors(char *file)
{
	int		fd;

	if (!file)
	{
		ft_putendl_fd("Error\nMiniRT: Could not retrieve file name", 2);
		return (1);
	}
	fd = open(file, O_WRONLY);
	if (errno == EISDIR)
	{
		ft_putstr_fd("Error\nMiniRT: Is a directory: ", 2);
		ft_putendl_fd(file, 2);
		return (1);
	}
	close(fd);
	return (0);
}

int	open_file_read(char *file, char *extension)
{
	int		fd;
	int		file_err;
	size_t	file_len;
	size_t	ext_len;

	file_err = check_file_errors(file);
	if (file_err)
		return (-1);
	file_len = ft_strlen(file);
	ext_len = ft_strlen(extension);
	if (file_len < ext_len + 2 || file[file_len - ext_len - 1] != '.' ||
		ft_strcmp(&file[file_len - ext_len], extension))
	{
		ft_putstr_fd("Error\nMiniRT: Not a valid file name: ", 2);
		ft_putendl_fd(file, 2);
		return (-1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_putendl_fd("Error", 2);
		perror("miniRT");
	}
	return (fd);
}
