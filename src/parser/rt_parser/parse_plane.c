/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/27 00:24:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "libft.h"

static int	parse_plane_elements(char **line_split, t_object *obj,
		t_parser_ctx *ctx, void *mlx)
{
	int	parse_result;
	int	split_len;

	split_len = get_str_array_length(line_split);
	parse_result = parse_vector(line_split[1], &(obj->position), "plane",
			ctx->line_nb);
	parse_result &= parse_vector(line_split[2], &(obj->direction), "plane",
			ctx->line_nb);
	parse_result &= parse_vector(line_split[3], &(obj->color), "plane",
			ctx->line_nb);
	obj->checker = ft_atoi(line_split[4]);
	if (split_len > 5 && ft_strlen(line_split[5]))
		parse_result &= parse_double(line_split[5], &(obj->specular),
				"plane", ctx->line_nb);
	if (split_len > 6 && ft_strlen(line_split[6]))
		parse_result &= parse_double(line_split[6], &(obj->shininess),
				"plane", ctx->line_nb);
	if (split_len > 7)
		parse_result &= parse_texture_file(line_split[7], obj, ctx, mlx);
	return (parse_result);
}

t_object	*parse_plane(char **line_split, t_parser_ctx *ctx, void *mlx)
{
	int			parse_result;
	t_object	*obj;

	obj = NULL;
	if (check_array_size(line_split, 5, "plane", ctx->line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "plane", ctx->line_nb);
		return (NULL);
	}
	obj->shininess = 1;
	parse_result = parse_plane_elements(line_split, obj, ctx, mlx);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->type = OBJ_PLANE;
	return (obj);
}
