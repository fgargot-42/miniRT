/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 19:14:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/08 01:45:43 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "object.h"

static int	get_material_element_index(char *mat_elem)
{
	unsigned long		i;
	static const char	*el_mat[] = {"Ns", "Ka", "Kd", "Ks", "Ke", "Ni", "d"};

	i = 0;
	while (i < sizeof(el_mat) / sizeof(char *))
	{
		if (!ft_strncmp(mat_elem, el_mat[i], ft_strlen(el_mat[i])))
			break ;
		i++;
	}
	if (i >= sizeof(el_mat) / sizeof(char *))
		return (-1);
	return (i);
}

static int	parse_material_line(char *line, t_list **mat_list, int line_nb)
{
	int					index;
	static t_material	*mat;
	static const t_obj_parser_fc	parse_elem[] = { parse_mat_exponent,
	parse_mat_ambient, parse_mat_diffuse, parse_mat_specular,
	parse_mat_emissive, parse_mat_density, parse_mat_opacity };

	if (!line)
	{
		if (mat)
			free(mat);
		mat = NULL;
		return (0);
	}
	if (!ft_strncmp(line, "newmtl", 6))
	{
		index = parse_new_material(line, mat_list, &mat);
		return (index);
	}
	index = get_material_element_index(line);
	if (index != -1)
		index = parse_elem[index](line, mat, line_nb);
	return (index);
}


static int	import_materials(char *mtl_file, t_list **mat_list, char *rt_path)
{
	int			fd;
	int			line_nb;
	int			status;
	char		*line;
	char		**split;
	t_material	*current;
	
	split = ft_split_by_whitespace(mtl_file);
	if (!split)
		return (0);
	mtl_file = ft_strjoin(rt_path, split[1]);
	fd = open_file_read(mtl_file, "mtl");
	free(mtl_file);
	free_str_array(split);
	if (fd < 0)
		return (0);
	current = NULL;
	line = get_next_line(fd);
	line_nb = 1;
	status = 1;
	while (line && status)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (line[0] && line[0] != '#')
			status = parse_material_line(line, mat_list, line_nb);
		free(line);
		line = get_next_line(fd);
		line_nb++;
	}
	if (!status)
		clear_gnl(fd, line);
	return (status);
}

static void	add_triangles_to_scene(t_scene *scene, t_list *triangles)
{
	t_list	*last;

	last = ft_lstlast(scene->objects);
	printf("%p\n", triangles->content);
	ft_lstiter(triangles, print_obj_list);
	if (last)
		last->next = triangles;
	else
		scene->objects = triangles;
}

static t_list	*get_material(char *line, t_list *materials)
{
	char **split;

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

static int	parse_obj_elements(int fd, char *rt_path, t_scene *scene)
{
	t_object_model		*obj;
	int					line_nb;
	int					status;
	char				*line;
	t_list				*current_mat;
	
	obj = ft_calloc(1, sizeof(t_object_model));
	if (!obj)
		return (0);
	line = get_next_line(fd);
	line_nb = 1;
	status = 1;
	current_mat = NULL;
	while (line && status)
	{	
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
		if (!status)
			printf("\t%s\n", line);
		free(line);
		line = get_next_line(fd);
		line_nb++;
	}
	if (status)
		add_triangles_to_scene(scene, obj->triangles);
	return (status);
}

int	parse_obj_file(char *rt_file, char *file, t_scene *scene)
{
	int		fd;
	int		parse_result;
	char	*rt_path;
	char	*rt_path_end;
	char	**split;

	rt_path = ft_strdup(rt_file);
	rt_path_end = ft_strrchr(rt_path, '/');
	if (rt_path_end)
		*(rt_path_end + 1) = '\0';
	split = ft_split_by_whitespace(file);
	if (!split)
		return (0);
	if (!split[0] || !split[1])
	{
		free_str_array(split);
		return (0);
	}
	rt_file = ft_strjoin(rt_path, split[1]);
	fd = open_file_read(rt_file, "obj");
	free(rt_file);
	free_str_array(split);
	if (fd == -1)
		return (0);
	parse_result = parse_obj_elements(fd, rt_path, scene);
	free(rt_path);
	return (2 * parse_result);
}
