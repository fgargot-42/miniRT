/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks_editor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:14:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/18 22:29:26 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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

static void	apply_tan_or_matrix(t_data *data)
{
	t_object	*obj;
	t_vec3		rotation;
	obj = data->scene->selected;
	if (obj && obj->type >= OBJ_CONE && obj->type != OBJ_TRIANGLE)
		obj->props.tan_angle = tan(obj->angle * M_PI / 180.0);

	
	rotation = obj->direction;
	obj->props.transform_axis = vec_get_matrix_rotation_z(vec3_normalize(rotation));
}

void	editor_mouse_down(int event, void *param)
{
	t_data		*data;
	t_slider	*s;
	int			mx;
	int			my;
	int			i;

	if (event != 1 || ((t_data *)param)->nb_sliders == 0)
		return ;
	data = (t_data *)param;
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
			apply_tan_or_matrix(data);
			draw_editor(data);
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
	data->dragging_slider = -1;
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
	apply_tan_or_matrix(data);
		

	draw_editor(data);
	draw(data);
}
