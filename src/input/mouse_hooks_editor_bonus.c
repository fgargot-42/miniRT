/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks_editor_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:14:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/15 00:28:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include <math.h>

static void	apply_tan_or_matrix(t_data *data)
{
	static const t_vec3	z_base = (t_vec3){{0.0, 0.0, 1.0}};
	t_object			*obj;
	t_mat3				dr;

	obj = data->scene->selected;
	if (!obj || obj->type < OBJ_PLANE || obj->type == OBJ_TRIANGLE)
		return ;
	if (obj->type > OBJ_CYLINDER)
		obj->props.tan_angle = tan(obj->angle * M_PI / 180.0);
	dr = vec_get_matrix_rotation_z(obj->rotation.z * M_PI / 180.0);
	dr = mat3_multiply(
			vec_get_matrix_rotation_x(obj->rotation.x * M_PI / 180.0), dr);
	dr = mat3_multiply(
			vec_get_matrix_rotation_y(obj->rotation.y * M_PI / 180.0), dr);
	obj->direction = vec_reverse_matrix(z_base, dr);
	obj->props.transform_axis = dr;
}

static int	handle_slider_click(t_data *data, t_slider *s, int mx, int my)
{
	double	t;

	if (!s || !s->value)
		return (0);
	if (mx < SLD_X || mx > SLD_X + SLD_W)
		return (0);
	if (my < s->y - 6 || my > s->y + SLD_H + 6)
		return (0);
	t = (double)(mx - SLD_X) / SLD_W;
	t = fmin(fmax(0.0, t), 1.0);
	*s->value = s->min + t * (s->max - s->min);
	apply_tan_or_matrix(data);
	if (data->scene->selected)
		draw_editor(data, mx, my);
	else
		draw_light_editor(data);
	mlx_mouse_move(data->mlx, data->editor, SLD_X + SLD_W / 2, s->y + SLD_H / 2);
	mlx_mouse_hide(data->mlx);
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
	data->render_scale = 1;
	mlx_mouse_show(data->mlx);
	draw(data);
}

void	editor_loop(void *param)
{
	t_data		*data;
	t_slider	*s;
	bool		edited;

	data = (t_data *)param;
	if (data->ui.dragging_slider < 0
		|| data->ui.dragging_slider >= data->ui.nb_sliders)
		return ;
	s = &data->ui.sliders[data->ui.dragging_slider];
	edited = apply_slider_x(s, data);
	if (!edited)
		return ;
	apply_tan_or_matrix(data);
	if (data->scene->selected)
		draw_editor(data, -1, -1);
	else
		draw_light_editor(data);
	if (s->affects_bvh)
		bvh_grow_all_to_include(data->scene->bvh, data->scene->selected);
	if (data->ui.dragging_slider != -1 && NB_THREADS <= 1)
		data->render_scale = RENDER_SCALE;
	draw(data);
}
