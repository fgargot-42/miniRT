/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 21:46:57 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/29 20:35:59 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
		data->scene->selected = NULL;
		mlx_mouse_get_pos(data->mlx, &mouse_x, &mouse_y);
		ray = camera_ray(data->scene->cam, mouse_x, mouse_y);
		if (hit_scene(data->scene, &ray, T_MAX, &hc))
		{
			hc.color = shade(&hc, data->scene, &ray);
			data->scene->selected = hc.object;
			printf("%d\n", data->scene->selected->type);
		}
		print_hit_info(data, hc, mouse_x, mouse_y);
	}
	if (mouse_event == 2 || mouse_event == 3)
	{
		mlx_mouse_move(data->mlx, data->win, WIDTH / 2, HEIGHT / 2);
		mlx_mouse_hide(data->mlx);
		if (mouse_event == 2)
			data->w_click_hold = 1;
		else
			data->r_click_hold = 1;
	}
}

void	mouse_up_hook(int mouse_event, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (mouse_event == 2 || mouse_event == 3)
	{
		mlx_mouse_show(data->mlx);
		if (mouse_event == 2)
			data->w_click_hold = 0;
		else
			data->r_click_hold = 0;
	}
}

void	mouse_wheel_hook(int mouse_event, void *param)
{
	t_data		*data;
	t_camera	*cam;
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




// editor
//
//

static void	apply_slider_x(t_slider *s, int mx)
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
 
void	editor_mouse_down(int event, void *param)
{
	t_data		*data;
	t_slider	*s;
	int			mx;
	int			my;
	int			i;
 
	if (event != 1)
		return ;
	data = (t_data *)param;
	if (data->nb_sliders == 0)
		return ;
	mlx_mouse_get_pos(data->mlx, &mx, &my);
	i = 0;
	while (i < data->nb_sliders)
	{
		s = &data->sliders[i];
		if (mx >= SLD_X && mx <= SLD_X + SLD_W
			&& my >= s->y - 6 && my <= s->y + SLD_H + 6)
		{
			data->dragging_slider = i;
			apply_slider_x(s, mx);
			draw_editor(data);
			draw(data);
			return ;
		}
		i++;
	}
}
 
void	editor_mouse_up(int event, void *param)
{
	t_data	*data;
 
	if (event != 1)
		return ;
	data = (t_data *)param;
	if (data->dragging_slider >= 0)
	{
		data->dragging_slider = -1;
		draw(data);
	}
}


void	editor_loop(void *param)
{
	t_data		*data;
	t_slider	*s;
	int			mx;
	int			my;
 
	data = (t_data *)param;
	if (data->dragging_slider < 0
		|| data->dragging_slider >= data->nb_sliders)
		return ;
	mlx_mouse_get_pos(data->mlx, &mx, &my);
	s = &data->sliders[data->dragging_slider];
	apply_slider_x(s, mx);
	draw_editor(data);
	draw(data);
}
 
