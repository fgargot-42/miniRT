/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:04:40 by mabarrer          #+#    #+#             */
/*   Updated: 2026/07/28 17:08:01 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "veclib.h"

void	setup_transform_sliders(t_data *data, t_object *obj)
{
	int						i;
	static const char		*position_labels[3] = {"pos.x", "pos.y", "pos.z"};
	static const char		*rotation_labels[3] = {"rot.x", "rot.y", "rot.z"};
	static const mlx_color	position_colors[3] = {
	{.r = 100, .g = 200, .b = 255, .a = 255},
	{.r = 100, .g = 255, .b = 130, .a = 255},
	{.r = 255, .g = 150, .b = 100, .a = 255}};

	i = 0;
	while (i < 3)
	{
		data->sliders[i] = (t_slider){.value = &obj->position.vec[i],
			.min = -SLD_POS_RANGE, .max = SLD_POS_RANGE,
			.label = position_labels[i], .col = position_colors[i], .affects_bvh=true};
		data->sliders[3 + i] = (t_slider){.value = &obj->direction.vec[i],
			.min = -1, .max = 1,
			.label = rotation_labels[i], .col = position_colors[i], .affects_bvh=true};
		i++;
	}
}

void	setup_color_sliders(t_data *data, t_object *obj)
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
		data->sliders[6 + i] = (t_slider){.value = &obj->color.vec[i],
			.min = 0.0, .max = 255.0,
			.label = color_labels[i], .col = color_colors[i], .affects_bvh=false};
		i++;
	}
}

void	setup_material_sliders(t_data *data, t_object *obj)
{
	static const char		*material_labels[2] = {"specular", "shininess"};
	static const mlx_color	material_colors[2] = {
	{.r = 180, .g = 180, .b = 255, .a = 255},
	{.r = 200, .g = 200, .b = 200, .a = 255}};

	data->sliders[9] = (t_slider){.value = &obj->specular,
		.min = 0.0, .max = 1.0,
		.label = material_labels[0], .col = material_colors[0], .affects_bvh=false};
	data->sliders[10] = (t_slider){.value = &obj->shininess,
		.min = 1.0, .max = 1000.0,
		.label = material_labels[1], .col = material_colors[1], .affects_bvh=false};
}

static void	get_property_values(t_object *obj, double **radius, double **height)
{
	*radius = NULL;
	*height = NULL;
	if (obj->type == OBJ_SPHERE)
		*radius = &obj->radius;
	else if (obj->type == OBJ_CYLINDER)
	{
		*radius = &obj->radius;
		*height = &obj->props.height;
	}
	else if (obj->type >= OBJ_CONE && obj->type != OBJ_TRIANGLE)
	{
		*radius = &obj->angle;
		*height = &obj->props.height;
	}
}

void	setup_property_sliders(t_data *data, t_object *obj)
{
	double					*radius;
	double					*height;
	static const char		*properties_labels[2] = {"radius", "height"};
	static const mlx_color	material_colors[2] = {
	{.r = 180, .g = 180, .b = 255, .a = 255},
	{.r = 200, .g = 200, .b = 200, .a = 255}};

	get_property_values(obj, &radius, &height);
	data->sliders[11] = (t_slider){.value = radius,
		.min = 0.0, .max = 90.0,
		.label = properties_labels[0], .col = material_colors[1], .affects_bvh=true};
	data->sliders[12] = (t_slider){.value = height,
		.min = 0.0, .max = 30.0,
		.label = properties_labels[1], .col = material_colors[1], .affects_bvh=true};
}
