/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:38:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/29 23:31:02 by fgargot          ###   ########.fr       */
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

static int	open_material_texture(char *line, t_material *mat, t_parser_ctx *ctx)
{
	int		status;
	char	**split;

	status = 1;
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 2, "mtl_texture", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	if (!strncmp(split[0], "map_Kd", 6))
		status &= parse_mat_color_tex(line, mat, ctx);
	free_str_array(split);
	return (status);
}

static int	parse_material_line(char *line, t_list **mat_list, t_parser_ctx *ctx)
{
	int								index;
	int								status;
	static t_material				*mat;
	static const t_obj_parser_fc	parse_elem[] = {parse_mat_exponent,
		parse_mat_ambient, parse_mat_diffuse, parse_mat_specular,
		parse_mat_emissive, parse_mat_density, parse_mat_opacity};

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
		status = parse_elem[index](line, mat, ctx->line_nb);
	else
		status = open_material_texture(line, mat, ctx);
	return (status);
}

static int	material_parse_loop(t_list **mat_list, t_parser_ctx *ctx)
{
	int			status;
	char		*line;

	line = get_next_line(ctx->fd);
	status = 1;
	while (line && status)
	{
		printf("Parse mtl file: line %d\n", ctx->line_nb);
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (line[0] && line[0] != '#')
			status = parse_material_line(line, mat_list, ctx);
		free(line);
		line = get_next_line(ctx->fd);
		ctx->line_nb++;
	}
	if (!status)
		clear_gnl(ctx->fd, line);
	return (status);
}

int	import_materials(char *mtl_file, t_list **mat_list, char *obj_path, void *mlx)
{
	int				status;
	char			**split;
	t_parser_ctx	ctx;

	ctx.line_nb = 1;
	ctx.mlx = mlx;
	split = ft_split_by_whitespace(mtl_file);
	if (!split)
		return (0);
	mtl_file = ft_strjoin(obj_path, split[1]);
	ctx.rt_path = obj_path;
	printf("Loading: %s\n", mtl_file);
	ctx.fd = open_file_read(mtl_file, "mtl");
	free(mtl_file);
	free_str_array(split);
	if (ctx.fd < 0)
		return (0);
	status = material_parse_loop(mat_list, &ctx);
	return (status);
}
