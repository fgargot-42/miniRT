/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/27 19:03:29 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "libft.h"

static int	parse_camera_elements(char **line_split, t_object *obj, int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(obj->position), "camera", line_nb);
	p_res &= parse_vector(line_split[2], &(obj->direction), "camera", line_nb);
	p_res &= parse_double(line_split[3], &(obj->props.fov), "camera", line_nb);
	obj->direction = vec3_normalize(obj->direction);
	return (p_res);
}

t_object	*parse_camera(char **line_split, t_parser_ctx *ctx, void *mlx)
{
	int			parse_result;
	t_object	*obj;

	(void)mlx;
	if (check_array_size(line_split, 4, "camera", ctx->line_nb))
		return (0);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "camera", ctx->line_nb);
		return (0);
	}
	parse_result = parse_camera_elements(line_split, obj, ctx->line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->props.pitch = 0.0;
	obj->props.yaw = 0.0;
	obj->type = OBJ_CAMERA;
	return (obj);
}
