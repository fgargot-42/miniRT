/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_slider_header_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 19:05:07 by mabarrer          #+#    #+#             */
/*   Updated: 2026/08/17 20:06:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

void	draw_slider(t_data *data, t_slider *s)
{
	double	t;
	int		filled_w;
	int		thumb_x;
	char	*buf;

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
	mlx_string_put(data->mlx, data->editor, PANEL_PAD, s->y + 6,
		(mlx_color){.rgba = COL_LABEL}, (char *)s->label);
	fill_rect(data, (t_vec2){{SLD_X + SLD_W + 8, s->y - 2}}, (t_vec2){{70, SLD_H
		+ 4}}, (mlx_color){.rgba = COL_BG});
	buf = ft_dtoa(*s->value, 2 * (!s->is_int));
	mlx_string_put(data->mlx, data->editor, SLD_X + SLD_W + 12, s->y + 6,
		(mlx_color){.rgba = COL_VALUE}, buf);
	free(buf);
}

static void	put_mouse_coordinates(t_data *data, t_row_info info, t_vec2 uv)
{
	char	*str_array[5];
	char	*print_str;

	print_str = NULL;
	str_array[0] = "u=";
	str_array[1] = ft_dtoa(uv.x, 1);
	str_array[2] = " v=";
	str_array[3] = ft_dtoa(uv.y, 1);
	str_array[4] = NULL;
	if (str_array[1] && str_array[3])
		print_str = ft_strjoin_array((const char **)str_array, "");
	if (print_str)
		put_row(data, info, print_str);
	if (str_array[1])
		free(str_array[1]);
	if (str_array[3])
		free(str_array[3]);
	if (print_str)
		free(print_str);
}

void	draw_object_header(t_data *data, double mouse_x, double mouse_y)
{
	int			y;
	t_object	*o;
	t_row_info	info;

	o = data->scene->selected;
	info.y = &y;
	info.win = data->editor;
	info.label = "type";
	info.val_col = (mlx_color){.rgba = COL_TYPE};
	y = PANEL_Y + TITLE_H + 8;
	put_section(data, data->editor, &y, "DATA ----");
	put_row(data, info, obj_type_name(o));
	info.label = "mouse";
	info.val_col = (mlx_color){.rgba = COL_VALUE};
	put_mouse_coordinates(data, info, (t_vec2){{.x = mouse_x, .y = mouse_y}});
}

void	draw_slider_group(t_data *d, int start, int end, int *y)
{
	int	i;

	i = start;
	while (i < end)
	{
		d->ui.sliders[i].y = *y;
		draw_slider(d, &d->ui.sliders[i]);
		*y += SLD_SPACING;
		i++;
	}
}
