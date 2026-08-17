/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 18:21:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:32:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_bonus.h"
#include "material_bonus.h"
#include "veclib.h"
#include "uv_bonus.h"
#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static t_vec2	map_uv(t_vec2 uv)
{
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);
	return (uv);
}

t_vec2	get_uv(t_object *obj, t_vec3 vec)
{
	t_vec2	result;

	ft_bzero(&result, sizeof(t_vec2));
	if (obj->type == OBJ_SPHERE)
		result = get_sphere_uv(obj, vec);
	if (obj->type == OBJ_TRIANGLE)
		result = get_triangle_uv(obj, vec);
	result = map_uv(result);
	return (result);
}

t_vec3	uv_to_color(t_object *obj, t_texture *tex, t_vec2 uv)
{
	t_vec3		col;
	mlx_color	pixel;

	while (uv.x > 1.0)
		uv.x--;
	while (uv.y > 1.0)
		uv.y--;
	if (obj->type == OBJ_TRIANGLE)
	{
		uv.x = uv.x * (tex->width - 1);
		uv.y = (1 - uv.y) * (tex->height - 1);
	}
	else
	{
		uv.x = (1 - uv.x) * (tex->width - 1);
		uv.y = uv.y * (tex->height - 1);
	}
	pixel = mlx_get_image_pixel(tex->mlx, tex->data, (int)uv.x, (int)uv.y);
	col.x = pixel.r;
	col.y = pixel.g;
	col.z = pixel.b;
	return (col);
}
