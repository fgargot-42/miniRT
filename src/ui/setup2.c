/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:52:13 by mabarrer          #+#    #+#             */
/*   Updated: 2026/06/12 19:57:52 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"



static const char		*g_color_labels[3] = {"col.r", "col.g", "col.b"};

static const mlx_color	g_color_colors[3] = {
{.r = 255, .g = 80, .b = 80, .a = 255},
{.r = 80, .g = 220, .b = 80, .a = 255},
{.r = 80, .g = 140, .b = 255, .a = 255}};

static const mlx_color	g_material_colors[2] = {
{.r = 180, .g = 180, .b = 255, .a = 255},
{.r = 200, .g = 200, .b = 200, .a = 255}};

static void	setup_ambient_color_sliders(t_data *data, t_vec3 *col)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		data->sliders[13 + i] = (t_slider){
			.value = ((double *)col) + i,
			.min = 0.0,
			.max = 255.0,
			.label = g_color_labels[i],
			.col = g_color_colors[i],
		};
		i++;
	}
}

void	setup_ambient_sliders(t_data *data, t_object *obj)
{
	t_vec3	*col;
	double	*intensity;

	(void)obj;
	col = &data->scene->ambient->color;
	intensity = &data->scene->ambient->props.intensity;
	setup_ambient_color_sliders(data, col);
	data->sliders[16] = (t_slider){
		.value = intensity,
		.min = 0.0,
		.max = 1.0,
		.label = "power",
		.col = g_material_colors[1],
	};
}
