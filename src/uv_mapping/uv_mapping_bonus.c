/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 18:21:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/25 01:50:28 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_bonus.h"
#include "material.h"
#include "veclib.h"
#include "uv.h"
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

t_vec3	uv_to_color(t_texture *tex, t_vec2 uv)
{
	t_vec3		col;
	mlx_color	pixel;
	int			x;
	int			y;

	x = (int)((1 - uv.x) * (tex->width - 1));
	y = (int)(uv.y * (tex->height - 1));
	pixel = mlx_get_image_pixel(tex->mlx, tex->data, x, y);
	col.x = pixel.r;
	col.y = pixel.g;
	col.z = pixel.b;
	return (col);
}

t_vec3	triangle_uv_to_color(t_object *obj, t_texture *tex, t_vec3 hit)
{
	t_vec2		uv;
	mlx_color	pixel;

	uv = get_triangle_uv(obj, hit);
	uv.x = uv.x * (tex->width - 1);
	uv.y = (1 - uv.y) * (tex->height - 1);
	pixel = mlx_get_image_pixel(tex->mlx, tex->data, (int)uv.x, (int)uv.y);
	return ((t_vec3){{pixel.r, pixel.g, pixel.b}});
}

t_texture	*load_texture(char *path, mlx_context mlx)
{
	t_texture	*tex;

	tex = ft_calloc(1, sizeof(t_texture));
	if (!tex)
		return (NULL);
	tex->data = mlx_new_image_from_file(mlx, path, &tex->width, &tex->height);
	if (!tex->data)
	{
		printf("Failed to load image\n");
		free(tex);
		return (NULL);
	}
	tex->mlx = mlx;
	mlx_get_image_pixel(tex->mlx, tex->data, 0, 0);
	printf("Image loaded: %s (%i x %i) at address %p\n", path,
		tex->width, tex->height, tex->data);
	return (tex);
}
