/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 00:42:26 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/21 18:50:35 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "hit.h"
#include "mlx.h"
#include "ui.h"
#include <stdarg.h>

void	setup_sliders(t_data *data)
{
	t_object	*obj;

	data->nb_sliders = 0;
	obj = data->scene->selected;
	if (!obj)
		return ;
	setup_transform_sliders(data, obj);
	setup_color_sliders(data, obj);
	setup_property_sliders(data, obj);
	setup_material_sliders(data, obj);
	setup_ambient_sliders(data, obj, 13);
	data->nb_sliders = 17;
}

void	draw_editor(t_data *d, double mx, double my)
{
	int	y;

	if (!d->editor || d->nb_sliders == 0)
		return ;
	mlx_clear_window(d->mlx, d->editor, (mlx_color){.rgba = COL_WHITE});
	fill_rect(d, (t_vec2){{0, 0}}, (t_vec2){{EDITOR_W, EDITOR_H}},
		(mlx_color){.rgba = COL_BG});
	header(d, mx, my);
	y = SLD_BASE_Y - 130;
	draw_group(d, (t_vec2){{0, 3}}, &y, "TRANSFORM -");
	draw_group(d, (t_vec2){{3, 6}}, &y, "ROTATION -");
	draw_group(d, (t_vec2){{6, 9}}, &y, "COLOR -----");
	draw_group(d, (t_vec2){{9, 11}}, &y, "MATERIAL -----");
	draw_group(d, (t_vec2){{11, 13}}, &y, "PROPERTIES -----");
	draw_ambient(d, &y);
	draw_hline(d, d->editor, y + 4);
	mlx_set_font_scale(d->mlx, "resources/font.ttf", 8.0f);
	mlx_string_put(d->mlx, d->editor, PANEL_PAD, y + 4,
		(mlx_color){.rgba = COL_FOOTER}, "fgargot && mabarrer | miniRT");
}

void	open_inspector(t_data *data, t_hit_record hit, double mouse_x,
		double mouse_y)
{
	int	panel_h;

	init_editor(data);
	mlx_clear_window(data->mlx, data->editor, (mlx_color){.rgba = COL_WHITE});
	if (!hit.object)
		return ;
	panel_h = TITLE_H + LINE_H * 25 + 60;
	mlx_set_font(data->mlx, "resources/font.ttf");
	fill_rect(data, (t_vec2){{PANEL_X, PANEL_Y}}, (t_vec2){{PANEL_W, panel_h}},
		(mlx_color){.rgba = COL_BG});
	fill_rect(data, (t_vec2){{PANEL_X, PANEL_Y}}, (t_vec2){{PANEL_W, TITLE_H}},
		(mlx_color){.rgba = COL_TITLEBAR});
	setup_sliders(data);
	draw_editor(data, mouse_x, mouse_y);
}
