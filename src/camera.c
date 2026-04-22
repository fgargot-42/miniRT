/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 19:15:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 21:19:14 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <mlx.h>

static void	apply_scale(double *u, double *v, double fov)
{
	double	scale;

	scale = tan((fov / 2.0) * M_PI / 180.0);
	*u = *u * scale;
	*v = *v * scale;
}

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
	t_ray	ray;

	u = (x / (double)WIDTH) * 2.0 - 1.0;
	v = (y / (double)HEIGHT) * 2.0 - 1.0;
	u = u * ((double)WIDTH / HEIGHT);
	apply_scale(&u, &v, cam->fov);
	ray.direction = get_ray_direction(cam, u, v);
	ray.origin = cam->position;
	return (ray);
}

static void	rotate_camera(t_vec3 *direction, double *yaw, double *pitch,
		t_vec2 mouse_delta)
{
	double	yaw_rad;
	double	pitch_rad;
	double	cos_pitch;

	*yaw -= mouse_delta.x * CAMERA_SENS;
	*pitch -= mouse_delta.y * CAMERA_SENS;
	if (fabs(*pitch) > 89.0)
		*pitch = 89.0 * (2 * (*pitch > 0) - 1);
	yaw_rad = *yaw * (M_PI / 180.0);
	pitch_rad = *pitch * (M_PI / 180.0);
	cos_pitch = cos(pitch_rad);
	direction->x = cos(yaw_rad) * cos_pitch;
	direction->y = sin(pitch_rad);
	direction->z = sin(yaw_rad) * cos_pitch;
	*direction = vec3_normalize(*direction);
}

void	mouse_loop(void *param)
{
	static int	is_moving = 0;
	t_data		*data;
	int			mouse_x;
	int			mouse_y;
	t_vec2		mouse_delta;

	data = (t_data *)param;
	if (!data->r_click_hold)
		return ;
	mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
	mouse_delta.x = mouse_x - data->last_mouse_x;
	mouse_delta.y = mouse_y - data->last_mouse_y;
	if (!mouse_delta.x && !mouse_delta.y && !is_moving)
		return ;
	is_moving = (mouse_delta.x || mouse_delta.y);
	data->render_scale = 1 + (NB_THREADS <= 1 && is_moving) * 7;
	if (is_moving)
		rotate_camera(&data->scene->cam->direction, &data->scene->cam->yaw,
			&data->scene->cam->pitch, mouse_delta);
	draw(data);
	if (!is_moving)
		return ;
	mlx_mouse_move(data->mlx, data->win, WIDTH / 2, HEIGHT / 2);
	data->last_mouse_x = WIDTH / 2;
	data->last_mouse_y = HEIGHT / 2;
}
