/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_light_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 19:09:28 by mabarrer          #+#    #+#             */
/*   Updated: 2026/08/10 17:27:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

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
		data->ui.sliders[slider_id + i] = (t_slider){.value = &pos->vec[i],
			.min = -SLD_POS_RANGE,
			.max = SLD_POS_RANGE,
			.label = pos_labels[i],
			.col = pos_colors[i]};
		i++;
	}
}

static void	setup_light_sliders_rgb(t_data *data, t_vec3 *col, int slider_id)
{
	int				i;
	const char		*color_labels[3] = {"col.r", "col.g", "col.b"};
	const mlx_color	color_colors[3] = {{.r = 255, .g = 80, .b = 80, .a = 255},
	{.r = 80, .g = 220, .b = 80, .a = 255},
	{.r = 80, .g = 140, .b = 255, .a = 255}};

	i = 0;
	while (i < 3)
	{
		data->ui.sliders[slider_id + i] = (t_slider){.value = &(col->vec[i]),
			.min = 0.0, .max = 255.0, .label = color_labels[i],
			.col = color_colors[i]};
		i++;
	}
}

void	setup_light_sliders(t_data *data)
{
	t_object	*light;

	light = (t_object *)data->scene->lights.array[data->ui.selected_light];
	setup_light_pos_sliders(data, &light->position, 4);
	setup_light_sliders_rgb(data, &light->color, 7);
	data->ui.sliders[10] = (t_slider){.value = &light->props.intensity,
		.min = 0.0, .max = 1.0, .label = "power",
		.col = (mlx_color){.r = 200, .g = 200, .b = 200, .a = 255}};
	data->ui.nb_sliders = 11;
}

static void	draw_light(t_data *d, int *y)
{
	char	*light_nb;
	char	*light_str;

	light_nb = ft_itoa(d->ui.selected_light);
	if (light_nb)
		light_str = ft_strjoin("LIGHT ", light_nb);
	else
		light_str = ft_strdup("LIGHT");
	if (light_nb)
		free(light_nb);
	if (light_str)
	{
		*y += 12;
		mlx_string_put(d->mlx, d->editor, PANEL_X + PANEL_PAD + 40, *y,
			(mlx_color){.rgba = COL_SECTION}, light_str);
		*y += LINE_H - 6;
		draw_hline(d, d->editor, *y);
		*y += 16;
		draw_slider_group(d, 4, 7, y);
		*y += 12;
		draw_slider_group(d, 7, 10, y);
		*y += 12;
		draw_slider_group(d, 10, 11, y);
		free(light_str);
	}
}

void	draw_light_editor(t_data *d)
{
	int		y;

	if (!d->editor || d->ui.nb_sliders == 0)
		return ;
	mlx_clear_window(d->mlx, d->editor, (mlx_color){.rgba = COL_WHITE});
	fill_rect(d, (t_vec2){{0, 0}}, (t_vec2){{EDITOR_W, EDITOR_H}},
		(mlx_color){.rgba = COL_BG});
	draw_header(d);
	y = PANEL_Y + TITLE_H + 8;
	draw_group(d, (t_vec2){{0, 4}}, &y, "AMBIENT -----");
	draw_button(d, &d->ui.buttons[0]);
	draw_button(d, &d->ui.buttons[1]);
	draw_light(d, &y);
	draw_footer(d);
}
