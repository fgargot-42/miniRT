/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 21:36:07 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"

static int	parse_sphere_optional_elements(char **line_split, t_object *obj,
	t_parser_ctx *ctx, void *mlx)
{
	int	nb_elements;
	int	p_res;

	nb_elements = get_str_array_length(line_split);
	p_res = 1;
	if (nb_elements > 4)
		p_res &= parse_double(line_split[4], &(obj->specular), "sphere",
				ctx->line_nb);
	if (nb_elements > 5)
		p_res &= parse_double(line_split[5], &(obj->shininess), "sphere",
				ctx->line_nb);
	if (nb_elements > 6)
		p_res &= parse_vector(line_split[6], &(obj->checker_color),
				"sphere", ctx->line_nb);
	if (nb_elements > 7)
		p_res &= parse_texture_file(line_split[7], obj, ctx, mlx);
	return (p_res);
}

static int	parse_sphere_elements(char **line_split, t_object *obj,
		t_parser_ctx *ctx, void *mlx)
{
	int	p_res;
	int	split_len;

	split_len = get_str_array_length(line_split);
	p_res = parse_vector(line_split[1], &(obj->position), "sphere",
			ctx->line_nb);
	p_res &= p_res && parse_double(line_split[2], &(obj->radius), "sphere",
			ctx->line_nb);
	p_res &= p_res && parse_vector(line_split[3], &(obj->color), "sphere",
			ctx->line_nb);
	if (p_res)
		obj->checker_color = obj->color;
	p_res &= parse_sphere_optional_elements(line_split, obj, ctx, mlx);
	return (p_res);
}

t_object	*parse_sphere(char **line_split, t_parser_ctx *ctx, void *mlx)
{
	int			parse_result;
	t_object	*obj;

	if (check_array_size(line_split, 4, "sphere", ctx->line_nb))
		return (0);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "sphere", ctx->line_nb);
		return (0);
	}
	obj->shininess = 1;
	parse_result = parse_sphere_elements(line_split, obj, ctx, mlx);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->radius /= 2;
	obj->direction = (t_vec3){{0, 0, 1}};
	obj->type = OBJ_SPHERE;
	return (obj);
}
