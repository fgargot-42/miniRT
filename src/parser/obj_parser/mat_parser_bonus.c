/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_parser_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:38:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/05 20:49:00 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "parser_bonus.h"
#include "miniRT_bonus.h"
#include <unistd.h>

static int	get_material_element_index(char *mat_elem)
{
	unsigned long		i;
	static const char	*el_mat[] = {
		"Ns", "Ka", "Kd", "Ks", "Ni", "d", "illum"};
	static const int	mat_size = sizeof(el_mat) / sizeof(char *);

	i = 0;
	while (ft_iswhitespace(*mat_elem))
		mat_elem++;
	while (i < mat_size)
	{
		if (!ft_strncmp(mat_elem, el_mat[i], ft_strlen(el_mat[i])))
			break ;
		i++;
	}
	if (i >= mat_size)
		return (-1);
	return (i);
}

static int	open_material_texture(char *line, t_material *mat,
	t_parser_ctx *ctx)
{
	int			status;
	char		**split;
	const char	whitespaces[] = {9, 10, 11, 12, 13, ' ' };

	status = 1;
	line = ft_strtrim(line, whitespaces);
	split = ft_split_by_whitespace(line);
	free(line);
	if (!split)
		return (0);
	if (check_array_size(split, 2, "mtl_texture", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	if (!strncmp(split[0], "map_Kd", 6))
		status &= parse_mat_tex(split, &mat->color_tex, ctx, split[0]);
	if (!strncmp(split[0], "map_Bump", 8))
		status &= parse_mat_tex(split, &mat->normal_tex, ctx, split[0]);
	if (!strncmp(split[0], "map_Ks", 6))
		status &= parse_mat_tex(split, &mat->spec_tex, ctx, split[0]);
	if (!strncmp(split[0], "map_d", 5))
		status &= parse_mat_tex(split, &mat->mask_tex, ctx, split[0]);
	free_str_array(split);
	return (status);
}

static int	parse_material_line(char *line, t_array *materials,
	t_parser_ctx *ctx)
{
	int								status;
	int								index;
	static const t_obj_parser_fc	parse_elem[] = {
		parse_mat_exponent, parse_mat_ambient, parse_mat_diffuse,
		parse_mat_specular, parse_mat_density,
		parse_mat_opacity, parse_mat_illum};

	if (!line)
		return (0);
	if (!ft_strncmp(line, "newmtl", 6))
	{
		status = parse_new_material(line, materials, &ctx->mat_parse);
		return (status);
	}
	index = get_material_element_index(line);
	if (index != -1)
		status = parse_elem[index](line, ctx->mat_parse, ctx->line_nb);
	else
		status = open_material_texture(line, ctx->mat_parse, ctx);
	return (status);
}

static int	material_parse_loop(t_array *materials, t_parser_ctx *ctx)
{
	int			status;
	char		*line;

	line = get_next_line(ctx->fd);
	status = 1;
	while (line && status)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (line[0] && line[0] != '#')
			status = parse_material_line(line, materials, ctx);
		free(line);
		line = get_next_line(ctx->fd);
		ctx->line_nb++;
	}
	if (!status)
		clear_gnl(ctx->fd, line);
	return (status);
}

int	import_materials(char *mtl_file, t_array *materials, char *obj_path,
	mlx_context	mlx)
{
	int				status;
	char			**split;
	t_parser_ctx	ctx;

	ctx.line_nb = 1;
	ctx.mlx = mlx;
	split = ft_split_by_whitespace(mtl_file);
	if (!split)
		return (0);
	if (*split[1] == '/')
		mtl_file = ft_strdup(split[1]);
	else
		mtl_file = ft_strjoin(obj_path, split[1]);
	ctx.rt_path = obj_path;
	printf("Loading: %s\n", mtl_file);
	ctx.fd = open_file_read(mtl_file, "mtl");
	free(mtl_file);
	free_str_array(split);
	if (ctx.fd < 0)
		return (0);
	status = material_parse_loop(materials, &ctx);
	close(ctx.fd);
	return (status);
}
