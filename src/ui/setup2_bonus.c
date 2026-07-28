/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:52:13 by mabarrer          #+#    #+#             */
/*   Updated: 2026/07/28 17:08:40 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

static void	setup_ambient_color_sliders(t_data *data, t_vec3 *col,
	int slider_id)
{
	int						i;
	static const char		*color_labels[3] = {"col.r", "col.g", "col.b"};
	static const mlx_color	color_colors[3] = {
	{.r = 255, .g = 80, .b = 80, .a = 255},
	{.r = 80, .g = 220, .b = 80, .a = 255},
	{.r = 80, .g = 140, .b = 255, .a = 255}};

	i = 0;
	while (i < 3)
	{
		data->sliders[slider_id + i] = (t_slider){.value = &(col->vec[i]),
			.min = 0.0, .max = 255.0,
			.label = color_labels[i], .col = color_colors[i], .affects_bvh=false};
		i++;
	}
}

void	setup_ambient_sliders(t_data *data, t_object *obj, int slider_id)
{
	(void)obj;
	setup_ambient_color_sliders(data, &data->scene->ambient->color, slider_id);
	data->sliders[slider_id + 3] = (t_slider){
		.value = &data->scene->ambient->props.intensity,
		.min = 0.0, .max = 1.0,
		.label = "power",
		.col = (mlx_color){.r = 200, .g = 200, .b = 200, .a = 255},
		.affects_bvh=false};
}
