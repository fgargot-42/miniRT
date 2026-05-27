/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 19:14:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/27 19:50:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "object.h"
#include <unistd.h>

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

static int	parse_obj_line(t_object_model *obj, char *line, char *obj_path,
	int line_nb)
{
	int				status;
	static t_list	*current_mat = NULL;

	status = 1;
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (!ft_strncmp(line, "mtllib", 6))
		status = import_materials(line, &obj->materials, obj_path);
	else if (!ft_strncmp(line, "usemtl", 6))
		current_mat = get_material(line, obj->materials);
	else if (!ft_strncmp(line, "vn", 2))
		status = parse_normal(line, &obj->normal_list, line_nb);
	else if (!ft_strncmp(line, "vt", 2))
		status = parse_texture(line, &obj->texture_uv_list, line_nb);
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

static int	parse_obj_elements(char **split, char *rt_path, t_scene *scene, t_object_model *obj)
{
	int		line_nb;
	int		status;
	char	*line;
	int		fd;
	char	*obj_file;

	obj_file = ft_strjoin(rt_path, split[2]);
	fd = open_file_read(obj_file, "obj");
	rt_path = ft_strrchr(obj_file, '/');
	if (rt_path)
		rt_path[1] = '\0';
	line = get_next_line(fd);
	line_nb = 1;
	status = (fd != -1);
	while (line && status)
	{
		status = parse_obj_line(obj, line, obj_file, line_nb);
		if (!status)
			printf("\t%s\n", line);
		free(line);
		line = get_next_line(fd);
		line_nb++;
	}
	if (status)
		ft_lstadd_back(&scene->objects, obj->triangles);
	free(obj_file);
	close(fd);
	return (status);
}

static int	parse_obj_texture_file(t_object_model *obj, char *rt_path,
		char *tex_file, void *mlx)
{
	char		*tex_path;
	t_texture	*tex;

	if (!tex_file || tex_file[0] == '\0')
		return (1);
	tex_path = ft_strjoin(rt_path, tex_file);
	if (!tex_path)
		return (0);
	tex = load_texture(tex_path, mlx);
	if (!tex)
		return (0);
	obj->tex = tex;
	return (1);
}

int	parse_obj_file(char *file, t_data *data, t_parser_ctx *ctx)
{
	int				parse_result;
	char			**split;
	t_object_model	*obj;

	parse_result = 1;
	split = ft_split_by_whitespace(file);
	if (!split)
		return (0);
	if (check_array_size(split, 3, "obj", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	obj = ft_calloc(1, sizeof(t_object_model));
	if (obj)
	{
		parse_vector(split[1], &obj->position, "obj", ctx->line_nb);
		if (split[3])
			parse_result = parse_obj_texture_file(obj, ctx->rt_path,
				split[3], data->mlx);
		parse_result &= parse_obj_elements(split, ctx->rt_path, data->scene, obj);
	}
	printf("Nb tris: %d\n", ft_lstsize(obj->triangles));
	free_str_array(split);
	return (2 * parse_result);
}
