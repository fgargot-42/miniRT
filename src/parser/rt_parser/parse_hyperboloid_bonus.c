/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hyperboloid_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/22 01:50:01 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"

static int	parse_hyperboloid_optional_elements(char **line_split,
	t_object *obj, t_parser_ctx *ctx, void *mlx)
{
	int	nb_elements;
	int	p_res;

	nb_elements = get_str_array_length(line_split);
	p_res = 1;
	obj->checker = (nb_elements > 10 && !is_ignored(line_split[10]));
	if (nb_elements > 8 && !is_ignored(line_split[8]))
		p_res &= parse_double(line_split[8], &(obj->specular), "hyperboloid",
				ctx->line_nb);
	if (nb_elements > 9 && !is_ignored(line_split[9]))
		p_res &= parse_double(line_split[9], &(obj->shininess), "hyperboloid",
				ctx->line_nb);
	if (nb_elements > 10 && !is_ignored(line_split[10]))
		p_res &= parse_vector(line_split[10], &(obj->checker_color),
				"hyperboloid", ctx->line_nb);
	if (nb_elements > 11 && !is_ignored(line_split[11]))
		p_res &= parse_texture_file(line_split[11], obj, ctx, mlx);
	if (nb_elements > 12 && !is_ignored(line_split[12]))
		p_res &= parse_spec_texture_file(line_split[12], obj, ctx, mlx);
	if (nb_elements > 13 && !is_ignored(line_split[13]))
		p_res &= parse_bump_texture_file(line_split[13], obj, ctx, mlx);
	return (p_res);
}

static int	parse_hyperboloid_elements(char **split, t_object *obj,
		t_parser_ctx *ctx, void *mlx)
{
	int	p_res;

	p_res = parse_vector(split[1], &(obj->position), "hyperboloid",
			ctx->line_nb);
	p_res &= parse_vector(split[2], &(obj->direction), "hyperboloid",
			ctx->line_nb);
	p_res &= parse_double(split[3], &(obj->radius), "hyperboloid",
			ctx->line_nb);
	p_res &= parse_double(split[4], &(obj->angle), "hyperboloid", ctx->line_nb);
	p_res &= parse_double(split[5], &(obj->props.height), "hyperboloid",
			ctx->line_nb);
	p_res &= parse_double(split[6], &(obj->props.depth), "hyperboloid",
			ctx->line_nb);
	p_res &= parse_vector(split[7], &(obj->color), "hyperboloid", ctx->line_nb);
	if (p_res)
		obj->checker_color = obj->color;
	p_res = parse_hyperboloid_optional_elements(split, obj, ctx, mlx);
	return (p_res);
}

t_object	*parse_hyperboloid(char **line_split, t_parser_ctx *ctx, void *mlx)
{
	int				parse_result;
	t_object		*obj;

	if (check_array_size(line_split, 7, "hyperboloid", ctx->line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "hyperboloid", ctx->line_nb);
		return (NULL);
	}
	obj->shininess = 1;
	parse_result = parse_hyperboloid_elements(line_split, obj, ctx, mlx);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->props.tan_angle = tan(obj->angle * M_PI / 180);
	obj->props.transform_axis = vec_get_matrix_rotation_z(obj->direction);
	obj->type = OBJ_HYPERBOLOID;
	return (obj);
}
