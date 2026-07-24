/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 22:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/25 01:48:13 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "miniRT_bonus.h"
#include "hit_bonus.h"

t_vec2	get_skybox_uv(t_vec3 dir)
{
	double	a;
	double	b;
	t_vec2	result;

	a = atan2(dir.z, dir.x);
	b = fmax(-1.0, fmin(1.0, dir.y));
	b = acos(b);
	result.x = (a + M_PI) / (2 * M_PI);
	result.y = b / M_PI;
	return (result);
}

t_vec3	draw_skybox(t_scene *scene, t_ray r)
{
	t_vec2		uv;
	t_vec3		uvcol;
	mlx_color	pixel;

	uv = get_skybox_uv(r.direction);
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);
	uv.x = (int)((1 - uv.x) * (scene->skybox->width - 1));
	uv.y = (int)(uv.y * (scene->skybox->height - 1));
	pixel = mlx_get_image_pixel(scene->skybox->mlx, scene->skybox->data,
		(int)uv.x, (int)uv.y);
	uvcol.x = pixel.r;
	uvcol.y = pixel.g;
	uvcol.z = pixel.b;
	return (uvcol);
}
