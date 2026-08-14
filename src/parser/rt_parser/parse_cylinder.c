/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 20:04:32 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "libft.h"

static int	parse_cylinder_elements(char **line_split, t_object *obj,
	t_parser_ctx *ctx)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(obj->position), "cylinder",
			ctx->line_nb);
	p_res &= parse_vector(line_split[2], &(obj->direction), "cylinder",
			ctx->line_nb);
	p_res &= parse_double(line_split[3], &(obj->radius), "cylinder",
			ctx->line_nb);
	p_res &= parse_double(line_split[4], &(obj->props.height), "cylinder",
			ctx->line_nb);
	p_res &= parse_vector(line_split[5], &(obj->color), "cylinder",
			ctx->line_nb);
	return (p_res);
}

t_object	*parse_cylinder(char **line_split, t_parser_ctx *ctx)
{
	int			parse_result;
	t_object	*obj;

	if (check_array_size(line_split, 6, "cylinder", ctx->line_nb))
		return (0);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "cylinder", ctx->line_nb);
		return (0);
	}
	parse_result = parse_cylinder_elements(line_split, obj, ctx);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->radius /= 2.0;
	obj->type = OBJ_CYLINDER;
	return (obj);
}
