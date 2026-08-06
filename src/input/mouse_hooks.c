/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 21:46:57 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/06 20:40:01 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"

void	apply_slider_x(t_slider *s, int mx)
{
	double	t;
	double	new_val;

	t = (double)(mx - SLD_X) / SLD_W;
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	new_val = s->min + t * (s->max - s->min);
	*s->value = new_val;
}

static void	mouse_enable_move_mode(t_data *data, int mouse_event)
{
	if (mouse_event == 2 || mouse_event == 3)
	{
		mlx_mouse_move(data->mlx, data->win, WIDTH / 2, HEIGHT / 2);
		mlx_mouse_hide(data->mlx);
		if (mouse_event == 2)
			data->w_click_hold = true;
		else
			data->r_click_hold = true;
	}
}

void	mouse_down_hook(int mouse_event, void *param)
{
	t_data			*data;
	int				mouse_x;
	int				mouse_y;
	t_ray			ray;
	t_hit_record	hc;

	data = (t_data *)param;
	ft_bzero(&hc, sizeof(hc));
	if (mouse_event == 1)
	{
		mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
		ray = camera_ray(data->scene->cam, mouse_x, mouse_y);
		if (hit_scene(data->scene, &ray, T_MAX, &hc))
		{
			if (data->scene->selected == hc.object)
				hc.object = NULL;
			data->scene->selected = hc.object;
			setup_sliders(data);
		}
		else
			data->scene->selected = NULL;
		open_inspector(data, hc, mouse_x, mouse_y);
		draw(data);
	}
	mouse_enable_move_mode(data, mouse_event);
}

void	mouse_up_hook(int mouse_event, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (mouse_event == 2 || mouse_event == 3)
	{
		mlx_mouse_show(data->mlx);
		if (mouse_event == 2)
			data->w_click_hold = false;
		else
			data->r_click_hold = false;
		data->render_scale = 1;
		draw(data);
	}
}

void	mouse_wheel_hook(int mouse_event, void *param)
{
	t_data		*data;
	t_object	*cam;
	t_vec3		fwd;

	data = (t_data *)param;
	cam = data->scene->cam;
	fwd = vec3_normalize(cam->direction);
	if (mouse_event == 1)
		cam->position = vec3_add(cam->position, vec3_scale(fwd, MOVE_STEP));
	else if (mouse_event == 2)
		cam->position = vec3_add(cam->position, vec3_scale(fwd, -MOVE_STEP));
	draw(data);
}
