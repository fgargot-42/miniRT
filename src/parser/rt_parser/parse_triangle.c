/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 22:53:49 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/27 00:25:49 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "libft.h"

static int	parse_triangle_elements(char **line_split, t_object *obj,
	t_parser_ctx *ctx, void *mlx)
{
	int	p_res;
	int	split_len;

	split_len = get_str_array_length(line_split);
	p_res = parse_vector(line_split[1], &(obj->props.a), "triangle",
			ctx->line_nb);
	p_res &= parse_vector(line_split[2], &(obj->props.b), "triangle",
			ctx->line_nb);
	p_res &= parse_vector(line_split[3], &(obj->props.c), "triangle",
			ctx->line_nb);
	p_res &= parse_vector(line_split[4], &(obj->color), "triangle",
			ctx->line_nb);
	obj->checker = ft_atoi(line_split[5]);
	if (split_len > 6 && ft_strlen(line_split[6]))
		p_res &= parse_double(line_split[6], &(obj->specular),
				"triangle", ctx->line_nb);
	if (split_len > 7 && ft_strlen(line_split[7]))
		p_res &= parse_double(line_split[7], &(obj->shininess),
				"triangle", ctx->line_nb);
	if (split_len > 8)
		p_res &= parse_texture_file(line_split[8], obj, ctx, mlx);
	return (p_res);
}

static void	init_triangle_properties(t_object *obj)
{
	obj->position = obj->props.a;
	obj->direction = vec3_normalize(vec3_cross(
		vec3_sub(obj->props.c, obj->props.a),
		vec3_sub(obj->props.b, obj->props.a)));
	obj->props.b = vec3_sub(obj->props.b, obj->props.a);
	obj->props.c = vec3_sub(obj->props.c, obj->props.a);
	obj->props.a = (t_vec3){0, 0, 0};
	obj->type = OBJ_TRIANGLE;
}

t_object	*parse_triangle(char **line_split, t_parser_ctx *ctx, void *mlx)
{
	int			parse_result;
	t_object	*obj;

	obj = NULL;
	if (check_array_size(line_split, 6, "triangle", ctx->line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "triangle", ctx->line_nb);
		return (NULL);
	}
	obj->shininess = 1;
	parse_result = parse_triangle_elements(line_split, obj, ctx, mlx);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	init_triangle_properties(obj);
	return (obj);
}
