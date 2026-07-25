/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:32:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/25 00:41:58 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "material.h"

static t_material	*create_material(t_data *data, t_texture **tex)
{
	t_material	*new_material;

	new_material = ft_calloc(1, sizeof(t_material));
	if (!new_material)
	{
		destroy_texture(tex);
		return (NULL);
	}
	new_material->density = 1.0;
	new_material->opacity = 1.0;
	ft_arrayadd_back(&data->scene->mat, new_material, destroy_material);
	return (new_material);
}

int	parse_texture_file(char *param, t_object *obj, t_parser_ctx *ctx)
{
	char		*tex_path;
	t_texture	*tex;

	if (!param || param[0] == '\0' || (param[0] == '.' && param[1] == '\0'))
		return (1);
	tex_path = ft_strjoin(ctx->rt_path, param);
	if (!tex_path)
		return (0);
	tex = load_texture(tex_path, ctx->data->mlx);
	free(tex_path);
	if (!tex)
		return (0);
	if (!obj->material)
		obj->material = create_material(ctx->data, &tex);
	if (!obj->material)
		return (0);
	obj->material->color_tex = tex;
	return (1);
}

int	parse_bump_texture_file(char *param, t_object *obj, t_parser_ctx *ctx)
{
	char		*tex_path;
	t_texture	*tex;

	if (!param || param[0] == '\0' || (param[0] == '.' && param[1] == '\0'))
		return (1);
	tex_path = ft_strjoin(ctx->rt_path, param);
	if (!tex_path)
		return (0);
	tex = load_texture(tex_path, ctx->data->mlx);
	free(tex_path);
	if (!tex)
		return (0);
	if (!obj->material)
		obj->material = create_material(ctx->data, &tex);
	if (!obj->material)
		return (0);
	obj->material->normal_tex = tex;
	return (1);
}

int	parse_spec_texture_file(char *param, t_object *obj, t_parser_ctx *ctx)
{
	char		*tex_path;
	t_texture	*tex;

	if (!param || param[0] == '\0' || (param[0] == '.' && param[1] == '\0'))
		return (1);
	tex_path = ft_strjoin(ctx->rt_path, param);
	if (!tex_path)
		return (0);
	tex = load_texture(tex_path, ctx->data->mlx);
	free(tex_path);
	if (!tex)
		return (0);
	if (!obj->material)
		obj->material = create_material(ctx->data, &tex);
	if (!obj->material)
		return (0);
	obj->material->spec_tex = tex;
	return (1);
}
