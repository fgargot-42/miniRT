/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 22:51:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/06 01:10:04 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_bonus.h"
#include "parser_bonus.h"
#include "miniRT_bonus.h"
#include "material.h"

static char	*get_object_type_str(t_object o)
{
	long		max;
	static char	*obj_names[] = {"ambient",
		"camera", "sky", "light", "plane",
		"sphere", "cylinder", "cone",
		"hyperboloid", "paraboloid", "triangle"};

	max = sizeof(obj_names) / sizeof(*obj_names);
	if (o.type < 0 || o.type >= max)
		return ("");
	return (obj_names[o.type]);
}

static int	parse_texture_elements(char **params, t_object *obj,
	t_parser_ctx *ctx, int i)
{
	int		nb_elements;
	int		p_res;

	nb_elements = get_str_array_length(params);
	p_res = 1;
	if (nb_elements > i && !is_ignored(params[i]))
		p_res &= parse_texture_file(params[i],
				&obj->material->color_tex, ctx);
	if (nb_elements > i + 1 && !is_ignored(params[i + 1]))
		p_res &= parse_texture_file(params[i + 1],
				&obj->material->spec_tex, ctx);
	if (nb_elements > i + 2 && !is_ignored(params[i + 2]))
		p_res &= parse_texture_file(params[i + 2],
				&obj->material->normal_tex, ctx);
	if (nb_elements > i + 3 && !is_ignored(params[i + 3]))
		p_res &= parse_texture_file(params[i + 3],
				&obj->material->mask_tex, ctx);
	return (p_res);
}

static int	parse_material_elements(char **params, t_object *obj,
	int line_nb, int i)
{
	char		*type;
	t_material	*mat;
	int			nb_elements;
	int			p_res;

	type = get_object_type_str(*obj);
	mat = obj->material;
	nb_elements = get_str_array_length(params);
	p_res = 1;
	if (nb_elements > i + 1 && !is_ignored(params[i + 1]))
		p_res &= parse_double(params[i + 1], &(mat->specular), type, line_nb);
	if (nb_elements > i + 2 && !is_ignored(params[i + 2]))
		p_res &= parse_double(params[i + 2], &(mat->shininess), type, line_nb);
	if (nb_elements > i + 3 && !is_ignored(params[i + 3]))
		p_res &= parse_double(params[i + 3], &(mat->opacity), type, line_nb);
	if (nb_elements > i + 4 && !is_ignored(params[i + 4]))
		p_res &= parse_double(params[i + 4], &(mat->density), type, line_nb);
	if (nb_elements > i + 5 && !is_ignored(params[i + 5]))
		p_res &= parse_double(params[i + 5],
				&(mat->reflectance), type, line_nb);
	return (p_res);
}

int	parse_optional_elements(char **params, t_object *obj,
	t_parser_ctx *ctx, int i)
{
	char	*type;
	int		nb_elements;
	int		p_res;

	if (!obj || !obj->material)
		return (0);
	p_res = 1;
	type = get_object_type_str(*obj);
	nb_elements = get_str_array_length(params);
	obj->checker_scale = 0;
	if (nb_elements > i && !is_ignored(params[i]))
	{
		p_res &= parse_vector(params[i], &(obj->checker_color),
				type, ctx->line_nb);
		obj->checker_scale = 1;
		if (nb_elements > i + 1 && !is_ignored(params[i + 1]))
			p_res &= parse_double(params[i + 1], &(obj->checker_scale),
					type, ctx->line_nb);
	}
	p_res = parse_material_elements(params, obj, ctx->line_nb, i + 1);
	p_res &= parse_texture_elements(params, obj, ctx, i + 7);
	return (p_res);
}
