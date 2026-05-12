/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 23:36:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/12 22:54:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

char	*get_directory_path(char *filepath)
{
	char	*path;
	char	*path_end;

	if (!filepath)
		return (NULL);
	path = ft_strdup(filepath);
	if (!path)
		return (NULL);
	path_end = ft_strrchr(path, '/');
	if (!path_end)
	{
		free(path);
		return (NULL);
	}
	*(path_end + 1) = '\0';
	return (path);
}

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
	if (file_len < ext_len + 2 || file[file_len - ext_len - 1] != '.'
		|| ft_strcmp(&file[file_len - ext_len], extension))
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
