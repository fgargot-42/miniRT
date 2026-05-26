/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/26 23:58:29 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "libft.h"

static int	parse_cylinder_elements(char **line_split, t_object *obj,
	t_parser_ctx *ctx, void *mlx)
{
	int	p_res;
	int	split_len;

	split_len = get_str_array_length(line_split);
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
	if (split_len > 6 && ft_strlen(line_split[6]))
		p_res &= parse_double(line_split[6], &(obj->specular), "cylinder",
				ctx->line_nb);
	if (split_len > 7 && ft_strlen(line_split[7]))
		p_res &= parse_double(line_split[7], &(obj->shininess), "cylinder",
				ctx->line_nb);
	if (split_len > 8)
		p_res &= parse_texture_file(line_split[8], obj, ctx, mlx);
	return (p_res);
}

t_object	*parse_cylinder(char **line_split, t_parser_ctx *ctx, void *mlx)
{
	int			parse_result;
	t_object	*obj;

	if (check_array_size(line_split, 4, "cylinder", ctx->line_nb))
		return (0);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "cylinder", ctx->line_nb);
		return (0);
	}
	obj->shininess = 1;
	parse_result = parse_cylinder_elements(line_split, obj, ctx, mlx);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->radius /= 2;
	obj->props.transform_axis = vec_get_matrix_rotation_z(obj->direction);
	obj->type = OBJ_CYLINDER;
	return (obj);
}
