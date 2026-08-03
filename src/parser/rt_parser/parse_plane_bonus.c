/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 20:28:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"

static int	parse_plane_elements(char **line_split, t_object *obj,
		t_parser_ctx *ctx)
{
	int	parse_result;

	parse_result = parse_vector(line_split[1], &(obj->position), "plane",
			ctx->line_nb);
	parse_result &= parse_vector(line_split[2], &(obj->direction), "plane",
			ctx->line_nb);
	parse_result &= parse_vector(line_split[3], &(obj->color), "plane",
			ctx->line_nb);
	if (parse_result)
		obj->checker_color = obj->color;
	parse_result &= parse_optional_elements(line_split, obj, ctx, 4);
	return (parse_result);
}

t_object	*parse_plane(char **line_split, t_parser_ctx *ctx)
{
	int			parse_result;
	t_object	*obj;

	obj = NULL;
	if (check_array_size(line_split, 4, "plane", ctx->line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "plane", ctx->line_nb);
		return (NULL);
	}
	obj->material = create_material(ctx->data);
	parse_result = parse_plane_elements(line_split, obj, ctx);
	if (!parse_result || !obj->material)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->type = OBJ_PLANE;
	return (obj);
}
