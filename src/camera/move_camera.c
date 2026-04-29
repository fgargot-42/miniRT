/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:36:54 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/29 18:41:33 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <mlx.h>

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

static void	translate_camera(t_camera *cam, t_vec2 mouse_delta)
{
	static t_vec3	world_up = (t_vec3){0, 1, 0};
	t_vec3			forward;
	t_vec3			right;

	mouse_delta = vec2_scale(mouse_delta, CAMERA_SENS * 0.1);
	forward = vec3_normalize(cam->direction);
	right = vec3_normalize(vec3_cross(world_up, forward));
	cam->position = vec3_add(cam->position, vec3_scale(right, mouse_delta.x));
	cam->position = vec3_add(cam->position, vec3_scale(world_up, -mouse_delta.y));
}

static int	move_camera(t_data *data, t_vec2 mouse_delta)
{
	static int	is_moving = 0;

	if (!mouse_delta.x && !mouse_delta.y && !is_moving)
		return (0);
	is_moving = (mouse_delta.x || mouse_delta.y);
	data->render_scale = 1 + (NB_THREADS <= 1 && is_moving) * 7;
	if (is_moving && data->r_click_hold)
		rotate_camera(&data->scene->cam->direction, &data->scene->cam->yaw,
			&data->scene->cam->pitch, mouse_delta);
	if (is_moving && data->w_click_hold)
		translate_camera(data->scene->cam, mouse_delta);
	draw(data);
	if (!is_moving || (!data->w_click_hold && !data->r_click_hold))
		return (0);
	return (1);
}

void	mouse_loop(void *param)
{
	int			moved;
	t_data		*data;
	int			mouse_x;
	int			mouse_y;
	t_vec2		mouse_delta;

	data = (t_data *)param;
	if (!data->r_click_hold && !data->w_click_hold)
		return ;
	mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
	mouse_delta.x = mouse_x - data->last_mouse_x;
	mouse_delta.y = mouse_y - data->last_mouse_y;
	moved = move_camera(data, mouse_delta);
	if (!moved)
		return ;
	mlx_mouse_move(data->mlx, data->win, WIDTH / 2, HEIGHT / 2);
	data->last_mouse_x = WIDTH / 2;
	data->last_mouse_y = HEIGHT / 2;
}
