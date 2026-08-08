/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks_editor_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:14:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/08 02:36:48 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include <math.h>

void	handle_button_click(t_data *data, t_button button, int mx, int my);

static void	apply_tan_or_matrix(t_data *data)
{
	t_object	*obj;
	t_vec3		rotation;

	obj = data->scene->selected;
	if (!obj || obj->type < OBJ_CYLINDER || obj->type == OBJ_TRIANGLE)
		return ;
	if (obj->type != OBJ_CYLINDER)
		obj->props.tan_angle = tan(obj->angle * M_PI / 180.0);
	rotation = obj->direction;
	obj->props.transform_axis
		= vec_get_matrix_rotation_z(vec3_normalize(rotation));
}

static int	handle_slider_click(t_data *data, t_slider *s, int mx, int my)
{
	if (!s || !s->value)
		return (0);
	if (mx < SLD_X || mx > SLD_X + SLD_W)
		return (0);
	if (my < s->y - 6 || my > s->y + SLD_H + 6)
		return (0);
	apply_slider_x(s, mx);
	apply_tan_or_matrix(data);
	if (data->scene->selected)
		draw_editor(data, mx, my);
	else
		draw_light_editor(data);
	return (1);
}

void	editor_mouse_down(int event, void *param)
{
	t_data		*data;
	int			mx;
	int			my;
	int			i;

	data = (t_data *)param;
	if (event != 1 || data->ui.nb_sliders == 0)
		return ;
	mlx_mouse_get_pos(data->mlx, &mx, &my);
	i = 0;
	while (i < data->ui.nb_sliders)
	{
		data->ui.dragging_slider = i;
		if (handle_slider_click(data, &data->ui.sliders[i], mx, my))
			break ;
		i++;
	}
	if (i >= data->ui.nb_sliders)
		data->ui.dragging_slider = -1;
	handle_button_click(data, data->ui.buttons[0], mx, my);
	handle_button_click(data, data->ui.buttons[1], mx, my);
}

void	editor_mouse_up(int event, void *param)
{
	t_data		*data;
	t_slider	*s;

	if (event != 1 || ((t_data *)param)->ui.dragging_slider == -1)
		return ;
	data = (t_data *)param;
	s = &data->ui.sliders[data->ui.dragging_slider];
	if (s->affects_bvh && data->scene->selected
		&& data->scene->selected->type != OBJ_PLANE)
		rebuild_bvh_tree(&data->scene->bvh, data->scene);
	data->ui.dragging_slider = -1;
}

void	editor_loop(void *param)
{
	t_data		*data;
	t_slider	*s;
	int			mx;
	int			my;

	data = (t_data *)param;
	if (data->ui.dragging_slider < 0
		|| data->ui.dragging_slider >= data->ui.nb_sliders)
		return ;
	mlx_mouse_get_pos(data->mlx, &mx, &my);
	s = &data->ui.sliders[data->ui.dragging_slider];
	apply_slider_x(s, mx);
	apply_tan_or_matrix(data);
	if (data->scene->selected)
		draw_editor(data, mx, my);
	else
		draw_light_editor(data);
	if (s->affects_bvh)
		bvh_grow_all_to_include(data->scene->bvh, data->scene->selected);
	draw(data);
}
