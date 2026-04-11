/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:55:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 01:33:11 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static int open_file_read(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		perror("miniRT");
	return (fd);
}

static void	print_parse_error(char *message, char *element)
{
	ft_putstr_fd("Error\n", 2);
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

static int	get_parse_element(char *id)
{
	static char	*id_elem[] = {"A", "C", "L", "sp", "pl", "cy", "co"};
	int					i;
	int					size;

	i = 0;
	size = sizeof(id_elem) / sizeof(char *);
	while (i < size)
	{
		if (!ft_strcmp(id, id_elem[i]))
			return (i);
		i++;
	}
	return (-1);
}

static int	parse_line(char *line, t_scene *scene)
{
	char						**line_split;
	int							i;
	static const t_parser_func	parse_elem[] = {parse_ambient,
		parse_camera, parse_light, parse_sphere, parse_plane, parse_cylinder,
		parse_cone};
	
	line_split = ft_split_by_whitespace(line);
	if (!line_split)
		return (0);
	if (!*line_split || *line_split[0] == '\0' || *line_split[0] == '#')
		return (1);
	i = get_parse_element(line_split[0]);
	if (i != -1)
		return (parse_elem[i](line_split, scene));
	print_parse_error("parser: wrong element identifier", line_split[0]);
	return (0);
}

int	parse_scene(char *file, t_scene *scene)
{
	int		fd;
	int		parse_status;
	char	*line;

	parse_status = 1;
	fd = open_file_read(file);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line && parse_status)
	{
		parse_status = parse_line(line, scene);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (!parse_status)
		get_next_line(fd);
	if (line)
		free(line);
	return (parse_status);
}
