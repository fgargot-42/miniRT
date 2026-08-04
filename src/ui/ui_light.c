/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_light.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 19:10:07 by mabarrer          #+#    #+#             */
/*   Updated: 2026/08/04 18:37:37 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	setup_light_pos_sliders(t_data *data, t_vec3 *pos, int slider_id)
{
	int				i;
	const char		*pos_labels[3] = {"pos.x", "pos.y", "pos.z"};
	const mlx_color	pos_colors[3] = {{.r = 100, .g = 200, .b = 255, .a = 255},
	{.r = 100, .g = 255, .b = 130, .a = 255},
	{.r = 255, .g = 150, .b = 100, .a = 255}};

	i = 0;
	while (i < 3)
	{
		data->sliders[slider_id + i] = (t_slider){.value = &pos->vec[i],
			.min = -SLD_POS_RANGE,
			.max = SLD_POS_RANGE,
			.label = pos_labels[i],
			.col = pos_colors[i]};
		i++;
	}
}

void	setup_light_sliders(t_data *data)
{
	t_object	*light;

	light = (t_object *)data->scene->light;
	setup_ambient_sliders(data, 0);
	setup_light_pos_sliders(data, &light->position, 4);
	data->sliders[7] = (t_slider){.value = &light->props.intensity, .min = 0.0,
		.max = 1.0, .label = "power", .col = (mlx_color){.r = 200, .g = 200,
		.b = 200, .a = 255}};
	data->nb_sliders = 8;
}

void	draw_light_editor(t_data *d)
{
	int		y;
	char	title[24];

	if (!d->editor || d->nb_sliders == 0)
		return ;
	mlx_clear_window(d->mlx, d->editor, (mlx_color){.rgba = COL_WHITE});
	fill_rect(d, (t_vec2){{0, 0}}, (t_vec2){{EDITOR_W, EDITOR_H}},
		(mlx_color){.rgba = COL_BG});
	y = 10;
	draw_group(d, (t_vec2){{0, 4}}, &y, "AMBIENT -----");
	snprintf(title, sizeof(title), "LIGHT");
	draw_group(d, (t_vec2){{4, 8}}, &y, title);
	draw_hline(d, d->editor, y + 4);
	mlx_set_font_scale(d->mlx, "resources/font.ttf", 8.0f);
	mlx_string_put(d->mlx, d->editor, PANEL_PAD, y + 4,
		(mlx_color){.rgba = COL_FOOTER}, "fgargot && mabarrer | miniRT");
	mlx_set_font_scale(d->mlx, "resources/font.ttf", 16.0f);
}
