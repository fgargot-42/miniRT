/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_slider_header_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 19:05:07 by mabarrer          #+#    #+#             */
/*   Updated: 2026/07/21 20:54:30 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

void	draw_slider(t_data *data, t_slider *s)
{
	double	t;
	int		filled_w;
	int		thumb_x;
	char	buf[32];

	if (!data->editor || !s->value)
		return ;
	t = slider_ratio(s);
	filled_w = (int)(t * SLD_W);
	thumb_x = SLD_X + filled_w;
	fill_rect(data, (t_vec2){{SLD_X, s->y - 1}}, (t_vec2){{SLD_W, SLD_H + 2}},
		slider_dim(s));
	if (filled_w > 0)
		fill_rect(data, (t_vec2){{SLD_X, s->y - 1}}, (t_vec2){{filled_w, SLD_H
			+ 2}}, s->col);
	fill_rect(data, (t_vec2){{thumb_x - 3, s->y - 4}}, (t_vec2){{6, SLD_H + 8}},
		(mlx_color){.rgba = COL_WHITE});
	mlx_string_put(data->mlx, data->editor, PANEL_PAD, s->y,
		(mlx_color){.rgba = COL_LABEL}, (char *)s->label);
	fill_rect(data, (t_vec2){{SLD_X + SLD_W + 8, s->y - 2}}, (t_vec2){{70, SLD_H
		+ 4}}, (mlx_color){.rgba = COL_BG});
	snprintf(buf, sizeof(buf), "%.2f", *s->value);
	mlx_string_put(data->mlx, data->editor, SLD_X + SLD_W + 8, s->y,
		(mlx_color){.rgba = COL_VALUE}, buf);
}

void	header(t_data *data, double mouse_x, double mouse_y)
{
	int			y;
	t_object	*o;

	o = data->scene->selected;
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 14.0f);
	mlx_string_put(data->mlx, data->editor, PANEL_X + PANEL_PAD, PANEL_Y + 16,
		(mlx_color){.rgba = COL_WHITE}, "miniRT INSPECTOR |      ;)");
	y = PANEL_Y + TITLE_H + 8;
	put_section(data, data->editor, &y, "DATA ----");
	put_row(data, (t_row_info){.y = &y, .win = data->editor, .label = "addr",
		.val_col = (mlx_color){.rgba = COL_ADDR}}, "%p", o);
	put_row(data, (t_row_info){.y = &y, .win = data->editor, .label = "type",
		.val_col = (mlx_color){.rgba = COL_TYPE}}, "%s", obj_type_name(o));
	put_row(data, (t_row_info){.y = &y, .win = data->editor, .label = "mouse",
		.val_col = (mlx_color){.rgba = COL_VALUE}}, "u=%.1f  v=%.1f", mouse_x,
		mouse_y);
}

void	draw_slider_group(t_data *d, int start, int end, int *y)
{
	int	i;

	i = start;
	while (i < end)
	{
		d->sliders[i].y = *y;
		draw_slider(d, &d->sliders[i]);
		*y += SLD_SPACING;
		i++;
	}
}
