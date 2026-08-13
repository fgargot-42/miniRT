/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 23:36:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/13 19:02:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

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

static bool	check_file_errors(char *file)
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
	if (fd > -1)
		close(fd);
	return (0);
}

static bool	check_file_extension(char *filepath, char *ext)
{
	char	*filename;
	size_t	file_len;
	size_t	ext_len;

	if (!filepath || !ext)
		return (false);
	filename = ft_strrchr(filepath, '/');
	if (filename)
		filename++;
	else
		filename = filepath;
	file_len = ft_strlen(filename);
	ext_len = ft_strlen(ext);
	if (file_len < ext_len + 1)
		return (false);
	if (filename[file_len - ext_len - 1] != '.')
		return (false);
	return (ft_strcmp(&filename[file_len - ext_len], ext) == 0);
}

int	open_file_read(char *file, char *extension)
{
	int		fd;
	bool	file_err;

	file_err = check_file_errors(file);
	if (file_err)
		return (-1);
	file_err = check_file_extension(file, extension);
	if (!file_err)
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

void	clear_gnl(int fd, char *line)
{
	if (line)
		free(line);
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	if (fd > -1)
		close(fd);
}
