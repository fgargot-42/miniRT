/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 00:42:26 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/08 01:05:53 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hit.h"
#include "miniRT.h"
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
	data->ui.nb_sliders = 15;
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
	draw_group(d, (t_vec2){{9, 11}}, &y, "PROPERTIES -----");
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
		setup_light_sliders(data);
		draw_light_editor(data);
	}
}
