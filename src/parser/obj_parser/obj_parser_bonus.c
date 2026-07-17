/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 19:14:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/18 00:23:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "object.h"
#include "miniRT_bonus.h"
#include <unistd.h>

static t_material	*get_material(char *line, t_array materials)
{
	char	**split;
	size_t	i;

	split = ft_split_by_whitespace(line);
	i = 0;
	if (!split)
		return (NULL);
	while (i < materials.len)
	{
		if (!ft_strcmp(((t_material *)materials.array[i])->name, split[1]))
			break ;
		i++;
	}
	free_str_array(split);
	return (materials.array[i]);
}

static bool	parse_obj_line(t_object_model *obj, char *line, char *obj_path,
	t_parser_ctx *ctx)
{
	bool				status;

	status = true;
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (!ft_strncmp(line, "mtllib", 6))
		status = import_materials(line, &obj->materials, obj_path, ctx->mlx);
	else if (!ft_strncmp(line, "usemtl", 6))
		ctx->current_mat = get_material(line, obj->materials);
	else if (!ft_strncmp(line, "vn", 2))
		status = parse_normal(line, &obj->vertex_normals, ctx->line_nb);
	else if (!ft_strncmp(line, "vt", 2))
		status = parse_texture(line, &obj->vertex_uv, ctx->line_nb);
	else if (!ft_strncmp(line, "v", 1))
		status = parse_vertex(line, &obj->vertices, ctx->line_nb);
	else if (!ft_strncmp(line, "f", 1))
		status = parse_face(line, obj, ctx->current_mat, ctx->line_nb);
	return (status);
}

static bool	parse_obj_elements(char **split, t_parser_ctx *ctx,
		t_scene *scene, t_object_model *obj)
{
	bool	status;
	char	*line;
	char	*obj_file;

	obj_file = ft_strjoin(ctx->rt_path, split[2]);
	ctx->fd = open_file_read(obj_file, "obj");
	line = ft_strrchr(obj_file, '/');
	if (line)
		line[1] = '\0';
	line = get_next_line(ctx->fd);
	status = (ctx->fd != -1);
	while (line && status)
	{
		status = parse_obj_line(obj, line, obj_file, ctx);
		if (!status)
			printf("\t%s\n", line);
		free(line);
		line = get_next_line(ctx->fd);
		ctx->line_nb++;
	}
	if (status)
		add_triangles_to_scene(scene, obj->triangles);
	free(obj_file);
	close(ctx->fd);
	return (status);
}

static void	init_ctx(t_parser_ctx *obj_ctx, t_parser_ctx *ctx, t_data *data)
{
	ft_bzero(obj_ctx, sizeof(t_parser_ctx));
	obj_ctx->mlx = data->mlx;
	obj_ctx->rt_path = ctx->rt_path;
	obj_ctx->line_nb = 1;
}

int	parse_obj_file(char *file, t_data *data, t_parser_ctx *ctx)
{
	int				status;
	char			**split;
	t_object_model	obj;
	t_parser_ctx	obj_ctx;

	status = 1;
	init_ctx(&obj_ctx, ctx, data);
	init_object_model(&obj);
	split = ft_split_by_whitespace(file);
	if (!split)
		return (0);
	if (check_array_size(split, 3, "obj", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	printf("Parsing object file: %s\n", split[2]);
	parse_vector(split[1], &obj.position, "obj", ctx->line_nb);
	if (split[3])
		status = parse_obj_tex_file(&obj, ctx->rt_path, split[3], data->mlx);
	status &= parse_obj_elements(split, &obj_ctx, data->scene, &obj);
	add_materials_to_scene(data->scene, obj.materials);
	destroy_object_model(&obj);
	free_str_array(split);
	return (2 * status);
}
