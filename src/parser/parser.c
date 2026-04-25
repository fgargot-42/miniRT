/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:55:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 19:44:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static int	get_parse_element(char *id)
{
	static char	*id_elem[] = {"A", "C", "L", "S", "sp", "pl", "cy", "co"};
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
	static const t_parser_func	parse_elem[] = {parse_ambient,
		parse_camera, parse_light, parse_sky, parse_sphere, parse_plane,
		parse_cylinder, parse_cone};

	line_split = ft_split_by_whitespace(line);
	if (!line_split || !obj)
		return (0);
	if (!*line_split || *line_split[0] == '\0' || *line_split[0] == '#')
	{
		free_str_array(line_split);
		return (2);
	}
	i = get_parse_element(line_split[0]);
	if (i != -1)
		*obj = parse_elem[i](line_split, line_nb);
	else
		print_parse_error("parser: wrong element identifier", line_split[0],
			line_nb);
	free_str_array(line_split);
	free(line);
	return (*obj != NULL);
}

static int	add_specials(void **dst, t_object *obj, char *elem, int line_nb)
{
	if (*dst)
	{
		print_parse_error("Duplicate element detected", elem, line_nb);
		return (0);
	}
	*dst = obj->object;
	return (1);
}

static int	add_element_to_scene(t_scene *scene, t_object **obj, int line_nb)
{
	int		status;
	t_list	*new_object;

	status = 1;
	if (!*obj)
		return (1);
	if ((*obj)->type == OBJ_SPHERE || (*obj)->type == OBJ_CYLINDER
		|| (*obj)->type == OBJ_CONE || (*obj)->type == OBJ_PLANE
		|| (*obj)->type == OBJ_LIGHT)
		{
			new_object = ft_lstnew(*obj);
			if (!new_object)
				return (0);
			if ((*obj)->type == OBJ_LIGHT)
				ft_lstadd_back(&scene->lights, new_object);
			else
				ft_lstadd_back(&scene->objects, new_object);
			return (1);
		}
	if ((*obj)->type == OBJ_AMBIENT)
		status = add_specials((void **)&scene->ambient, *obj, "ambient", line_nb);
	if ((*obj)->type == OBJ_CAMERA)
		status = add_specials((void **)&scene->cam, *obj, "camera", line_nb);
	if ((*obj)->type == OBJ_SKY)
		status = add_specials((void **)&scene->sky, *obj, "sky", line_nb);
	free(*obj);
	*obj = NULL;
	return (status);
}

int	parse_scene(char *file, t_scene *scene)
{
	int			fd;
	t_object	*obj;
	char		*line;
	int			line_nb;
	int			status;

	line_nb = 0;
	status = 1;
	fd = open_file_read(file);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line && status)
	{
		line_nb++;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		status = parse_line(line, line_nb, &obj);
		line = get_next_line(fd);
		if (status == 1)
			status = add_element_to_scene(scene, &obj, line_nb);
	}
	clear_gnl(fd, line);
	return (status);
}
