/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:55:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/13 20:09:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"
#include "object_bonus.h"
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

static int	parse_line(char *line, t_parser_ctx *ctx)
{
	char						**line_split;
	int							i;
	const char					sep[] = {9, 10, 11, 12, 13, ' ', '\0'};
	static const t_parser_func	parse_elem[] = {parse_ambient, parse_camera,
		parse_light, parse_sky, parse_sphere, parse_plane, parse_cylinder,
		parse_cone, parse_hyperboloid, parse_paraboloid, parse_triangle};

	line_split = rt_parser_split(line, sep);
	if (!line_split)
		return (0);
	if (!*line_split || *line_split[0] == '\0' || *line_split[0] == '#')
	{
		free_str_array(line_split);
		return (2);
	}
	i = get_parse_element(line_split[0]);
	printf("Adding object to scene: %s\t(id=%i)\n", line_split[0], i);
	if (i != -1)
		ctx->obj = parse_elem[i](line_split, ctx);
	else
		print_parse_error("parser: wrong element identifier", line_split[0],
			ctx->line_nb);
	set_euler_rotation(ctx->obj);
	free_str_array(line_split);
	return (ctx->obj != NULL);
}

static int	add_element_to_scene(t_scene *scene, t_parser_ctx *ctx)
{
	int		status;

	status = 1;
	if (!ctx->obj)
		return (1);
	if (ctx->obj->type >= OBJ_LIGHT)
	{
		if (ctx->obj->type == OBJ_LIGHT)
			ft_arrayadd_back(&scene->lights, ctx->obj, free_object);
		else
			ft_arrayadd_back(&scene->objects, ctx->obj, free_object);
		return (1);
	}
	if (ctx->obj->type == OBJ_AMBIENT)
		status = add_specials(&scene->ambient, ctx, "ambient");
	else if (ctx->obj->type == OBJ_CAMERA)
		status = add_specials(&scene->cam, ctx, "camera");
	else if (ctx->obj->type == OBJ_SKY)
		status = add_specials(&scene->sky, ctx, "sky");
	return (status);
}

static int	parse_scene_loop(t_parser_ctx *ctx)
{
	char		*line;
	int			status;

	line = get_next_line(ctx->fd);
	status = 1;
	while (line && status)
	{
		ctx->obj = NULL;
		ctx->line_nb++;
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (!ft_strncmp(line, "obj", 3))
			status = parse_obj_file(line, ctx->data, ctx);
		else if (!ft_strncmp(line, "SB", 2))
			status = parse_skybox(line, ctx);
		else
			status = parse_line(line, ctx);
		free(line);
		line = get_next_line(ctx->fd);
		if (status == 1)
			status = add_element_to_scene(ctx->data->scene, ctx);
	}
	clear_gnl(ctx->fd, line);
	return (status);
}

bool	parse_scene(char *file, t_data *data)
{
	int				status;
	t_parser_ctx	parser_ctx;
	double			parse_start_s;
	double			parse_end_s;

	parser_ctx.line_nb = 0;
	parser_ctx.data = data;
	parser_ctx.mlx = data->mlx;
	parser_ctx.fd = open_file_read(file, "rt");
	if (parser_ctx.fd == -1)
		return (0);
	parser_ctx.rt_path = get_directory_path(file);
	parse_start_s = get_time();
	status = parse_scene_loop(&parser_ctx);
	parse_end_s = get_time();
	free(parser_ctx.rt_path);
	if (status)
		printf("Parsed %zu objects in %.3f ms\n", data->scene->objects.len,
			1000 * (parse_end_s - parse_start_s));
	return (status != 0);
}
