/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 21:24:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/25 00:08:42 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "miniRT_bonus.h"
#include "parser_bonus.h"

static char	*get_tex_path(char *rt_path, char *tex_path)
{
	char	*res_path;

	if (!rt_path || !tex_path)
		return (NULL);
	if (tex_path[0] == '/')
		res_path = ft_strdup(tex_path);
	else
		res_path = ft_strjoin(rt_path, tex_path);
	return (res_path);
}

int	parse_mat_color_tex(char *line, t_material *mat, t_parser_ctx *ctx)
{
	char	**split;
	char	*path;

	if (!line || !mat)
		return (0);
	if (mat->color_tex)
		return (1);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 2, "map_Kd", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	path = get_tex_path(ctx->rt_path, split[1]);
	free_str_array(split);
	mat->color_tex = load_texture(path, ctx->mlx);
	free(path);
	return (mat->color_tex != NULL);
}

int	parse_mat_normal_tex(char *line, t_material *mat, t_parser_ctx *ctx)
{
	char	**split;
	char	*path;
	int		i;

	if (!line || !mat)
		return (0);
	if (mat->normal_tex)
		return (1);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 2, "map_Bump", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	i = 1;
	while (split[i + 1])
		i++;
	path = get_tex_path(ctx->rt_path, split[i]);
	free_str_array(split);
	mat->normal_tex = load_texture(path, ctx->mlx);
	free(path);
	return (mat->normal_tex != NULL);
}

int	parse_mat_spec_tex(char *line, t_material *mat, t_parser_ctx *ctx)
{
	char	**split;
	char	*path;
	int		i;

	if (!line || !mat)
		return (0);
	if (mat->spec_tex)
		return (1);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 2, "map_Ks", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	i = 1;
	while (split[i + 1])
		i++;
	path = get_tex_path(ctx->rt_path, split[i]);
	free_str_array(split);
	mat->spec_tex = load_texture(path, ctx->mlx);
	free(path);
	return (mat->spec_tex != NULL);
}

int	parse_obj_tex_file(t_object_model *obj, char *rt_path,
		char *tex_file, void *mlx)
{
	char		*tex_path;
	t_texture	*tex;

	if (!tex_file || tex_file[0] == '\0')
		return (1);
	tex_path = get_tex_path(rt_path, tex_file);
	if (!tex_path)
		return (0);
	tex = load_texture(tex_path, mlx);
	if (!tex)
		return (0);
	obj->tex = tex;
	return (1);
}
