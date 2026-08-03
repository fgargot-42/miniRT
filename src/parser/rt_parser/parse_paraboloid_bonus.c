/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_paraboloid_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 20:29:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"

static int	parse_paraboloid_elements(char **line_split, t_object *obj,
		t_parser_ctx *ctx)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(obj->position), "paraboloid",
			ctx->line_nb);
	p_res &= parse_vector(line_split[2], &(obj->direction), "paraboloid",
			ctx->line_nb);
	p_res &= parse_double(line_split[3], &(obj->angle), "paraboloid",
			ctx->line_nb);
	p_res &= parse_double(line_split[4], &(obj->props.height), "paraboloid",
			ctx->line_nb);
	p_res &= parse_vector(line_split[5], &(obj->color), "paraboloid",
			ctx->line_nb);
	if (p_res)
		obj->checker_color = obj->color;
	p_res &= parse_optional_elements(line_split, obj, ctx, 6);
	return (p_res);
}

t_object	*parse_paraboloid(char **line_split, t_parser_ctx *ctx)
{
	int				parse_result;
	t_object		*obj;

	if (check_array_size(line_split, 5, "paraboloid", ctx->line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "paraboloid", ctx->line_nb);
		return (NULL);
	}
	obj->material = create_material(ctx->data);
	parse_result = parse_paraboloid_elements(line_split, obj, ctx);
	if (!parse_result || !obj->material)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->props.tan_angle = tan(obj->angle * M_PI / 180);
	obj->props.transform_axis = vec_get_matrix_rotation_z(obj->direction);
	obj->type = OBJ_PARABOLOID;
	return (obj);
}
