/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 19:15:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 20:37:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <mlx.h>


static t_vec3	get_ray_direction(t_camera *cam, double u, double v)
{
	t_vec3			forward;
	static t_vec3	world_up = (t_vec3){0, 1, 0};
	t_vec3			right;
	t_vec3			up;
	t_vec3			dir;

	forward = vec3_normalize(cam->direction);
	right = vec3_normalize(vec3_cross(forward, world_up));
	up = vec3_cross(right, forward);
	dir = vec3_normalize(vec3_add(vec3_add(vec3_scale(right, -u),
					vec3_scale(up, -v)), forward));
	return (dir);
}

t_ray	camera_ray(t_camera *cam, int x, int y)
{
	double	u;
	double	v;
	double	scale;
	t_ray	ray;

	u = (x / (double)WIDTH) * 2.0 - 1.0;
	v = (y / (double)HEIGHT) * 2.0 - 1.0;
	u = u * ((double)WIDTH / HEIGHT);
	scale = tan((cam->fov / 2.0) * M_PI / 180.0);
	u *= scale;
	v *= scale;
	ray.direction = get_ray_direction(cam, u, v);
	ray.origin = cam->position;
	return (ray);
}
