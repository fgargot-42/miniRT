/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:52:13 by mabarrer          #+#    #+#             */
/*   Updated: 2026/08/14 21:38:10 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

static void	setup_ambient_color_sliders(t_data *data, t_vec3 *col,
		int slider_id)
{
	int				i;
	const char		*color_labels[3] = {"col.r", "col.g", "col.b"};
	const mlx_color	color_colors[3] = {
	{.r = 255, .g = 80, .b = 80, .a = 255},
	{.r = 80, .g = 220, .b = 80, .a = 255},
	{.r = 80, .g = 140, .b = 255, .a = 255}};

	i = 0;
	while (i < 3)
	{
		data->ui.sliders[slider_id + i] = (t_slider){.value = &(col->vec[i]),
			.min = 0.0, .max = 255.0, .snap = 1, .label = color_labels[i],
			.col = color_colors[i], .affects_bvh = false, .is_int = true};
		i++;
	}
}

void	setup_ambient_sliders(t_data *data, int slider_id)
{
	setup_ambient_color_sliders(data, &data->scene->ambient->color, slider_id);
	data->ui.sliders[slider_id
		+ 3] = (t_slider){.value = &data->scene->ambient->props.intensity,
		.min = 0.0, .max = 1.0, .snap = 0.05, .label = "power",
		.col = (mlx_color){.r = 200, .g = 200, .b = 200, .a = 255},
		.affects_bvh = false, .is_int = false};
}

void	setup_checker_sliders(t_data *data, t_object *obj, int slider_id)
{
	int						i;
	static const char		*checker_labels[3] = {"chk.r", "chk.g", "chk.b"};
	static const mlx_color	checker_colors[3] = {
	{.r = 255, .g = 140, .b = 140, .a = 255},
	{.r = 140, .g = 255, .b = 160, .a = 255},
	{.r = 140, .g = 180, .b = 255, .a = 255}};

	i = 0;
	while (i < 3)
	{
		data->ui.sliders[slider_id + i] = (t_slider){
			.value = &obj->checker_color.vec[i],
			.min = 0.0, .max = 255.0, .snap = 1, .label = checker_labels[i],
			.col = checker_colors[i], .affects_bvh = false, .is_int = true};
		i++;
	}
	data->ui.sliders[slider_id + 3] = (t_slider){.value = &obj->checker_scale,
		.min = 0, .max = 10.0, .snap = 0.1, .label = "chk.scale",
		.col = (mlx_color){.r = 220, .g = 220, .b = 140, .a = 255},
		.affects_bvh = false, .is_int = false};
}
