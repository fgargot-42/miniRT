/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 02:06:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:31:23 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hit_bonus.h"
#include "miniRT_bonus.h"
#include "mlx.h"
#include "ui.h"
#include <stdarg.h>

void	setup_sliders(t_data *data)
{
	t_object	*obj;

	data->ui.nb_sliders = 0;
	obj = data->scene->selected;
	if (!obj)
		return ;
	setup_transform_sliders(data, obj);
	setup_color_sliders(data, obj);
	setup_property_sliders(data, obj);
	setup_material_sliders(data, obj);
	setup_checker_sliders(data, obj, 17);
	data->ui.nb_sliders = 21;
}

void	draw_editor(t_data *d, double mx, double my)
{
	int	y;

	if (!d->editor || d->ui.nb_sliders == 0)
		return ;
	mlx_clear_window(d->mlx, d->editor, (mlx_color){.rgba = COL_BG});
	fill_rect(d, (t_vec2){{0, 0}}, (t_vec2){{EDITOR_W, EDITOR_H}},
		(mlx_color){.rgba = COL_BG});
	draw_header(d);
	draw_object_header(d, mx, my);
	y = SLD_BASE_Y - 130;
	draw_group(d, (t_vec2){{0, 3}}, &y, "TRANSFORM -");
	draw_group(d, (t_vec2){{3, 6}}, &y, "ROTATION -");
	draw_group(d, (t_vec2){{6, 9}}, &y, "COLOR -----");
	draw_group(d, (t_vec2){{9, 14}}, &y, "MATERIAL -----");
	draw_group(d, (t_vec2){{14, 17}}, &y, "PROPERTIES -----");
	draw_group(d, (t_vec2){{17, 21}}, &y, "CHECKER ----");
	draw_footer(d);
}

void	open_inspector(t_data *data, t_hit_record hit, double mouse_x,
		double mouse_y)
{
	(void)hit;
	init_editor(data);
	mlx_clear_window(data->mlx, data->editor, (mlx_color){.rgba = COL_WHITE});
	if (data->scene->selected)
	{
		setup_sliders(data);
		draw_editor(data, mouse_x, mouse_y);
	}
	else
	{
		setup_ambient_sliders(data, 0);
		setup_light_sliders(data);
		setup_button_event(&data->ui.buttons[0], "<", (t_vec2){{10, 135}},
			select_prev_light);
		setup_button_event(&data->ui.buttons[1], ">", (t_vec2){{270, 135}},
			select_next_light);
		draw_light_editor(data);
	}
}
