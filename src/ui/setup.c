/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:04:40 by mabarrer          #+#    #+#             */
/*   Updated: 2026/05/20 00:39:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static const char		*g_position_labels[3] = {"pos.x", "pos.y", "pos.z"};
static const char		*g_rotation_labels[3] = {"rot.x", "rot.y", "rot.z"};

static const char		*g_color_labels[3] = {"col.r", "col.g", "col.b"};

static const char		*g_material_labels[2] = {"specular", "shininess"};

static const char		*g_properties_labels[2] = {"radius", "height"};
static const mlx_color	g_position_colors[3] = {
{.r = 100, .g = 200, .b = 255, .a = 255},
{.r = 100, .g = 255, .b = 130, .a = 255},
{.r = 255, .g = 150, .b = 100, .a = 255}};

static const mlx_color	g_color_colors[3] = {
{.r = 255, .g = 80, .b = 80, .a = 255},
{.r = 80, .g = 220, .b = 80, .a = 255},
{.r = 80, .g = 140, .b = 255, .a = 255}};

static const mlx_color	g_material_colors[2] = {
{.r = 180, .g = 180, .b = 255, .a = 255},
{.r = 200, .g = 200, .b = 200, .a = 255}};

void	setup_transform_sliders(t_data *data, t_object *obj)
{
	int		i;
	t_vec3	*pos;
	t_vec3	*rotation;

	pos = &obj->position;
	rotation = &obj->direction;
	i = 0;
	while (i < 3)
	{
		data->sliders[i] = (t_slider){
			.value = ((double *)pos) + i,
			.min = -SLD_POS_RANGE,
			.max = SLD_POS_RANGE,
			.label = g_position_labels[i],
			.col = g_position_colors[i],
		};
		i++;
	}
	i = 0;
	while (i < 3)
	{
		data->sliders[3 + i] = (t_slider){
			.value = ((double *)rotation) + i,
			.min = -1,
			.max = 1,
			.label = g_rotation_labels[i],
			.col = g_position_colors[i],
		};
		i++;
	}
}

void	setup_color_sliders(t_data *data, t_object *obj)
{
	int		i;
	t_vec3	*col;

	col = &obj->color;
	i = 0;
	while (i < 3)
	{
		data->sliders[6 + i] = (t_slider){
			.value = ((double *)col) + i,
			.min = 0.0,
			.max = 255.0,
			.label = g_color_labels[i],
			.col = g_color_colors[i],
		};
		i++;
	}
}

void	setup_material_sliders(t_data *data, t_object *obj)
{
	data->sliders[9] = (t_slider){
		.value = &obj->specular,
		.min = 0.0,
		.max = 1.0,
		.label = g_material_labels[0],
		.col = g_material_colors[0],
	};
	data->sliders[10] = (t_slider){
		.value = &obj->shininess,
		.min = 1.0,
		.max = 1000.0,
		.label = g_material_labels[1],
		.col = g_material_colors[1],
	};
}

void	setup_property_sliders(t_data *data, t_object *obj)
{
	double	*radius;
	double	*height;

	radius = NULL;
	height = NULL;
	if (obj->type == OBJ_SPHERE)
		radius = &obj->radius;
	else if (obj->type == OBJ_CYLINDER)
	{
		radius = &obj->radius;
		height = &obj->props.height;
	}
	else if (obj->type >= OBJ_CONE && obj->type != OBJ_TRIANGLE)
	{
		radius = &obj->angle;
		height = &obj->props.height;
	}
	data->sliders[11] = (t_slider){
		.value = radius,
		.min = 0.0,
		.max = 90.0,
		.label = g_properties_labels[0],
		.col = g_material_colors[1],
	};
	data->sliders[12] = (t_slider){
		.value = height,
		.min = 0.0,
		.max = 30.0,
		.label = g_properties_labels[1],
		.col = g_material_colors[1],
	};
}

void	setup_ambient_sliders(t_data *data, t_object *obj)
{
	int		i;
	t_vec3	*col;
	double	*intensity;

	(void)obj;
	col = &data->scene->ambient->color;
	intensity = &data->scene->ambient->props.intensity;
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
	data->sliders[13 + i] = (t_slider){
		.value = intensity,
		.min = 0.0,
		.max = 1.0,
		.label = "power",
		.col = g_material_colors[i],
	};
}
