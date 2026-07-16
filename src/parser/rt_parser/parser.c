/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:55:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 00:47:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static int	get_parse_element(char *id)
{
	static char	*id_elem[] = {"A", "C", "L", "sp", "pl", "cy"};
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
	static const t_parser_func	parse_elem[] = {parse_ambient, parse_camera,
		parse_light, parse_sphere, parse_plane, parse_cylinder};

	line_split = ft_split_by_whitespace(line);
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
	free_str_array(line_split);
	return (ctx->obj != NULL);
}

static int	add_element_to_scene(t_scene *scene, t_parser_ctx *ctx)
{
	int		status;

	status = 1;
	if (!ctx->obj)
		return (1);
	if (ctx->obj->type > OBJ_LIGHT)
	{
		ft_arrayadd_back(&scene->objects, ctx->obj, free_object);
		return (1);
	}
	if (ctx->obj->type == OBJ_AMBIENT)
		status = add_specials(&scene->ambient, ctx, "ambient");
	if (ctx->obj->type == OBJ_CAMERA)
		status = add_specials(&scene->cam, ctx, "camera");
	if (ctx->obj->type == OBJ_LIGHT)
		status = add_specials(&scene->light, ctx, "light");
	return (status);
}

static int	parse_scene_loop(t_data *data, t_parser_ctx *ctx)
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
		status = parse_line(line, ctx);
		free(line);
		line = get_next_line(ctx->fd);
		if (status == 1)
			status = add_element_to_scene(data->scene, ctx);
	}
	clear_gnl(ctx->fd, line);
	return (status);
}

int	parse_scene(char *file, t_data *data)
{
	int				status;
	t_parser_ctx	parser_ctx;

	parser_ctx.line_nb = 0;
	parser_ctx.fd = open_file_read(file, "rt");
	if (parser_ctx.fd == -1)
		return (0);
	parser_ctx.rt_path = get_directory_path(file);
	status = parse_scene_loop(data, &parser_ctx);
	free(parser_ctx.rt_path);
	return (status);
}
