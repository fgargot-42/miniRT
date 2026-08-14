/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks_editor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:14:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 19:44:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	apply_tan_or_matrix(t_data *data)
{
	static const t_vec3	z_base = (t_vec3){{0.0, 0.0, 1.0}};
	t_object			*obj;
	t_mat3				dr;

	obj = data->scene->selected;
	if (!obj || obj->type != OBJ_CYLINDER)
		return ;
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
			return ;
		i++;
	}
	data->ui.dragging_slider = -1;
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
	draw(data);
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
	data->render_scale = 1;
	if (data->ui.dragging_slider != -1)
		data->render_scale = RENDER_SCALE;
	draw(data);
}
