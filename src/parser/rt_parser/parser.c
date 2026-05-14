/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:55:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/14 20:30:27 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "object.h"
#include <fcntl.h>
#include <unistd.h>

static int	get_parse_element(char *id)
{
	static char	*id_elem[] = {"A", "C", "L", "S", "sp", "pl",
		"cy", "co", "hy", "pa", "tr"};
	int			i;
	int			size;

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

static int	parse_line(char *line, int line_nb, t_object **obj)
{
	char						**line_split;
	int							i;
	static const t_parser_func	parse_elem[] = {parse_ambient, parse_camera,
		parse_light, parse_sky, parse_sphere, parse_plane, parse_cylinder,
		parse_cone, parse_hyperboloid, parse_paraboloid, parse_triangle};

	line_split = ft_split_by_whitespace(line);
	if (!line_split || !obj)
		return (0);
	if (!*line_split || *line_split[0] == '\0' || *line_split[0] == '#')
	{
		free_str_array(line_split);
		return (2);
	}
	i = get_parse_element(line_split[0]);
	printf("Adding object to scene: %s\t(id=%i)\n", line_split[0], i);
	if (i != -1)
		*obj = parse_elem[i](line_split, line_nb);
	else
		print_parse_error("parser: wrong element identifier", line_split[0],
			line_nb);
	free_str_array(line_split);
	return (*obj != NULL);
}

static int	parse_scene_loop(int rt_fd, t_scene *scene, char *file)
{
	char		*line;
	int			status;
	int			line_nb;
	t_object	*obj;

	line = get_next_line(rt_fd);
	line_nb = 0;
	status = 1;
	while (line && status)
	{
		obj = NULL;
		line_nb++;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (!ft_strncmp(line, "obj", 3))
			status = parse_obj_file(file, line, scene);
		else
			status = parse_line(line, line_nb, &obj);
		free(line);
		line = get_next_line(rt_fd);
		if (status == 1)
			status = add_element_to_scene(scene, &obj, line_nb);
	}
	clear_gnl(rt_fd, line);
	return (status);
}

int	parse_scene(char *file, t_scene *scene)
{
	int			fd;
	int			status;

	fd = open_file_read(file, "rt");
	if (fd == -1)
		return (0);
	status = parse_scene_loop(fd, scene, file);
	return (status);
}
