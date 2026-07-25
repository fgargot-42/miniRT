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

int	parse_mat_tex(char *line, t_texture **tex, t_parser_ctx *ctx, char *param)
{
	char	*path;

	if (!line || !param)
		return (0);
	if (!tex)
		return (1);
	while (*line && !ft_iswhitespace(*line))
		line++;
	while (*line && ft_iswhitespace(*line))
		line++;
	if (!*line)
	{
		print_parse_error("missing parameter(s)", param, ctx->line_nb);
		return (0);
	}
	path = get_tex_path(ctx->rt_path, line);
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
