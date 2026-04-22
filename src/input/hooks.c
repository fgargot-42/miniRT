/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 22:44:49 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 19:15:05 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <SDL2/SDL_scancode.h>

#define MOVE_STEP 0.5

void	window_hook(int event, void *param)
{
	mlx_context	mlx;

	(void)event;
	mlx = (mlx_context)param;
	if (event == 0)
		mlx_loop_end(mlx);
}

static void	move_camera(int scancode, t_data *data)
{
	static t_vec3	w_up = (t_vec3){0, 1, 0};
	t_camera		*cam;
	t_vec3			fwd;
	t_vec3			right;

	cam = data->scene->cam;
	fwd = vec3_normalize(cam->direction);
	right = vec3_normalize(vec3_cross(fwd, w_up));
	if (scancode == 26)
		cam->position = vec3_add(cam->position, vec3_scale(fwd, MOVE_STEP));
	else if (scancode == 22)
		cam->position = vec3_add(cam->position, vec3_scale(fwd, -MOVE_STEP));
	else if (scancode == 4)
		cam->position = vec3_add(cam->position, vec3_scale(right, MOVE_STEP));
	else if (scancode == 7)
		cam->position = vec3_add(cam->position, vec3_scale(right, -MOVE_STEP));
	else if (scancode == SDL_SCANCODE_E)
		cam->position = vec3_add(cam->position, vec3_scale(w_up, MOVE_STEP));
	else if (scancode == SDL_SCANCODE_Q)
		cam->position = vec3_add(cam->position, vec3_scale(w_up, -MOVE_STEP));
}

void	key_hook(int scancode, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (scancode == SDL_SCANCODE_ESCAPE)
		mlx_loop_end(data->mlx);
	move_camera(scancode, data);
	if (scancode == SDL_SCANCODE_TAB)
	{
		if (data->render_scale == 1)
			data->render_scale = 8;
		else
			data->render_scale = 1;
	}
	draw(data);
}

void	attach_hooks(t_data *data)
{
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook,
		data->mlx);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_hook, data);
}
