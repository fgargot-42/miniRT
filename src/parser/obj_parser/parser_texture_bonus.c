/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 21:24:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/28 01:16:30 by fgargot          ###   ########.fr       */
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

static bool	parse_tex_vector(char **split, t_vec3* vec, char* param, int line_nb)
{
	bool	parse_result;

	parse_result = parse_double(split[1], &vec->x, param, line_nb);
	parse_result &= parse_double(split[2], &vec->y, param, line_nb);
	parse_result &= parse_double(split[3], &vec->z, param, line_nb);
	return (parse_result);
}

static bool	parse_tex_option(char **split, int *i, t_material *mat, int line_nb)
{
	bool	parse_result;

	parse_result = true;
	if (!ft_strcmp(split[*i], "-s"))
	{
		parse_result = parse_tex_vector(&split[*i], &(mat->spec_color), "-s", line_nb);
		*i += 4;
	}
	else if (!ft_strcmp(split[*i], "-o") || !ft_strcmp(split[*i], "-t"))
		*i += 4;
	else
		*i += 2;
	return (parse_result);
}

int	parse_mat_tex(char **split, t_texture **tex, t_parser_ctx *ctx, char *param)
{
	char	*path;
	int		i;
	char	*filename;

	if (!split || !split[0] || !split[1] || !param)
		return (0);
	if (!tex)
		return (1);
	i = 1;
	while (split[i][0] == '-')
		parse_tex_option(split, &i, ctx->mat_parse, ctx->line_nb);
	if (!split[i][0])
	{
		print_parse_error("missing parameter(s)", param, ctx->line_nb);
		return (0);
	}
	filename = ft_strjoin_array((const char **)&split[i], " ");
	if (!filename)
		return (0);
	path = get_tex_path(ctx->rt_path, filename);
	free(filename);
	*tex = load_texture(path, ctx->mlx);
	free(path);
	return (*tex != NULL);
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
