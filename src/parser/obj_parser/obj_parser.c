/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 19:14:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/26 17:24:15 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "object.h"

static t_list	*get_material(char *line, t_list *materials)
{
	char	**split;

	split = ft_split_by_whitespace(line);
	if (!split)
		return (NULL);
	while (materials)
	{
		if (!ft_strcmp(((t_material *)materials->content)->name, split[1]))
			break ;
		materials = materials->next;
	}
	free_str_array(split);
	return (materials);
}

static int	parse_obj_line(t_object_model *obj, char *line, char *rt_path,
	int line_nb)
{
	int				status;
	static t_list	*current_mat = NULL;

	status = 1;
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (!ft_strncmp(line, "mtllib", 6))
		status = import_materials(line, &obj->materials, rt_path);
	else if (!ft_strncmp(line, "usemtl", 6))
		current_mat = get_material(line, obj->materials);
	else if (!ft_strncmp(line, "vn", 2))
		status = parse_normal(line, &obj->normal_list, line_nb);
	else if (!ft_strncmp(line, "vt", 2))
		status = parse_texture(line, &obj->texture_list, line_nb);
	else if (!ft_strncmp(line, "v", 1))
		status = parse_vertex(line, &obj->vertex_list, line_nb);
	else if (!ft_strncmp(line, "f", 1))
	{
		if (current_mat)
			status = parse_face(line, obj,
					(t_material *)current_mat->content, line_nb);
		else
			status = parse_face(line, obj, NULL, line_nb);
	}
	return (status);
}

static int	parse_obj_elements(int fd, char *rt_path, t_scene *scene, t_vec3 obj_pos)
{
	t_object_model		*obj;
	int					line_nb;
	int					status;
	char				*line;

	obj = ft_calloc(1, sizeof(t_object_model));
	if (!obj)
		return (0);
	obj->position = obj_pos;
	line = get_next_line(fd);
	line_nb = 1;
	status = 1;
	while (line && status)
	{
		status = parse_obj_line(obj, line, rt_path, line_nb);
		if (!status)
			printf("\t%s\n", line);
		free(line);
		line = get_next_line(fd);
		line_nb++;
	}
	if (status)
		ft_lstadd_back(&scene->objects, obj->triangles);
	return (status);
}

int	parse_obj_file(char *rt_file, char *file, t_scene *scene, int line_nb)
{
	int		fd;
	int		parse_result;
	char	*rt_path;
	char	**split;
	t_vec3	obj_position;

	rt_path = get_directory_path(rt_file);
	split = ft_split_by_whitespace(file);
	if (!split)
		return (0);
	if (check_array_size(split, 3, "obj", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_vector(split[1], &obj_position, "obj", line_nb);
	rt_file = ft_strjoin(rt_path, split[2]);
	fd = open_file_read(rt_file, "obj");
	free_str_array(split);
	if (fd == -1)
		return (0);
	parse_result = parse_obj_elements(fd, rt_path, scene, obj_position);
	free(rt_path);
	return (2 * parse_result);
}
