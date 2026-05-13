/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 00:42:26 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/13 18:41:52 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "mlx.h"
#include "ui.h"
#include <stdarg.h>
#include <stdio.h>

void	init_editor(t_data *data)
{
	mlx_window_create_info	info;

	if (data->editor)
		return ;
	info = (mlx_window_create_info){
		.title = "inspector",
		.width = EDITOR_W,
		.height = EDITOR_H,
	};
	data->editor = mlx_new_window(data->mlx, &info);
	if (!data->editor)
		exit(1);
	mlx_set_window_size(data->mlx, data->editor, EDITOR_W, EDITOR_H);
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEDOWN, editor_mouse_down,
			data);
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEUP, editor_mouse_up, data);
}

static const char	*obj_type_name(int type)
{
	static const char	*names[] = {
		[OBJ_SPHERE] = "SPHERE",
		[OBJ_PLANE] = "PLANE",
		[OBJ_CYLINDER] = "CYLINDER",
		[OBJ_CONE] = "CONE",
		[OBJ_HYPERBOLOID] = "HYPERBOLOID",
		[OBJ_PARABOLOID] = "PARABOLOID",
		[OBJ_TRIANGLE] = "TRIANGLE",
	};
	if (type < 0 || type >= (int)(sizeof(names) / sizeof(*names)))
		return ("UNKNOWN");
	return (names[type]);
}

void	setup_sliders(t_data *data)
{
	t_object				*obj;
	t_vec3					*pos;
	t_vec3					*col;
	double					*spec;
	double					*shin;
	double					*radius;
	double					*height;
	int						i;
	static const char		*position_labels[3] = {"pos.x", "pos.y", "pos.z"};
	static const char		*color_labels[3] = {"col.r", "col.g", "col.b"};
	static const char		*material_labels[2] = {"specular", "shininess"};
	static const char		*properties_labels[2] = {"radius", "height"};
	static const mlx_color	position_colors[3] = {{.r = 100, .g = 200, .b = 255,
			.a = 255}, {.r = 100, .g = 255, .b = 130, .a = 255}, {.r = 255,
			.g = 150, .b = 100, .a = 255}};
	static const mlx_color	color_colors[3] = {{.r = 255, .g = 80, .b = 80,
			.a = 255}, {.r = 80, .g = 220, .b = 80, .a = 255}, {.r = 80,
			.g = 140, .b = 255, .a = 255}};
	static const mlx_color	material_colors[2] = {{.r = 180, .g = 180, .b = 255,
			.a = 255}, {.r = 200, .g = 200, .b = 200, .a = 255}};

	data->nb_sliders = 0;
	obj = data->scene->selected;
	if (!obj)
		return ;
	pos = &obj->position;
	col = &obj->color;
	spec = &obj->specular;
	shin = &obj->shininess;
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
	if (!pos || !col)
		return ;
	// POSITION (0–2)
	i = 0;
	while (i < 3)
	{
		data->sliders[i] = (t_slider){
			.value = ((double *)pos) + i,
			.min = -SLD_POS_RANGE,
			.max = SLD_POS_RANGE,
			.label = position_labels[i],
			.col = position_colors[i],
		};
		i++;
	}
	// COLOR (3–5)
	i = 0;
	while (i < 3)
	{
		data->sliders[3 + i] = (t_slider){
			.value = ((double *)col) + i,
			.min = 0.0,
			.max = 255.0,
			.label = color_labels[i],
			.col = color_colors[i],
		};
		i++;
	}
	// MATERIAL (6–7)
	data->sliders[6] = (t_slider){
		.value = spec,
		.min = 0.0,
		.max = 1.0,
		.label = material_labels[0],
		.col = material_colors[0],
	};
	data->sliders[7] = (t_slider){
		.value = shin,
		.min = 1.0,
		.max = 1000.0,
		.label = material_labels[1],
		.col = material_colors[1],
	};
	data->sliders[8] = (t_slider){
		.value = radius,
		.min = 0.0,
		.max = 90.0,
		.label = properties_labels[0],
		.col = material_colors[1],
	};
	data->sliders[9] = (t_slider){
		.value = height,
		.min = 0.0,
		.max = 30.0,
		.label = properties_labels[1],
		.col = material_colors[1],
	};
	data->nb_sliders = 10;
}

/*
   PANEL_PAD           SLD_X          SLD_X+SLD_W    SLD_X+SLD_W+50
   label               [====filled====|----empty----]    value
   ^ curseur
 */
static void	draw_slider(t_data *data, t_slider *s)
{
	double		t;
	int			filled_w;
	int			thumb_x;
	char		buf[32];
	mlx_color	dim = (mlx_color){.r = s->col.r / 5, .g = s->col.g / 5,
			.b = s->col.b / 5, .a = 255};

	if (!s->value || s->max == s->min)
		return ;
	t = (*s->value - s->min) / (s->max - s->min);
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	filled_w = (int)(t * SLD_W);
	thumb_x = SLD_X + filled_w;
	dim = (mlx_color){.r = s->col.r / 5, .g = s->col.g / 5, .b = s->col.b / 5,
		.a = 255};
	fill_rect(data, (t_vec2){SLD_X, s->y - 1}, (t_vec2){SLD_W, SLD_H + 2}, dim);
	if (filled_w > 0)
		fill_rect(data, (t_vec2){SLD_X, s->y - 1}, (t_vec2){filled_w, SLD_H
			+ 2}, s->col);
	fill_rect(data, (t_vec2){thumb_x - 3, s->y - 4}, (t_vec2){6, SLD_H + 8},
		(mlx_color){.rgba = COL_WHITE});
	mlx_string_put(data->mlx, data->editor, PANEL_PAD, s->y,
		(mlx_color){.rgba = COL_LABEL}, (char *)s->label);
	snprintf(buf, sizeof(buf), "%.2f", *s->value);
	mlx_string_put(data->mlx, data->editor, SLD_X + SLD_W + 8, s->y,
		(mlx_color){.rgba = COL_VALUE}, buf);
}

void	draw_editor(t_data *data)
{
	int	y;
	int	i;

	if (!data->editor || data->nb_sliders == 0)
		return ;
	fill_rect(data, (t_vec2){0, SLD_BASE_Y - 44}, (t_vec2){EDITOR_W, EDITOR_H
		- (SLD_BASE_Y - 44)}, (mlx_color){.rgba = COL_BG});
	y = SLD_BASE_Y - 44;
	put_section(data, data->editor, &y, "TRANSFORM -");
	i = 0;
	while (i < 3)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}
	y += 12;
	put_section(data, data->editor, &y, "COLOR -----");
	i = 3;
	while (i < 6)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}
	y += 12;
	put_section(data, data->editor, &y, "MATERIAL -----");
	i = 6;
	while (i < 8)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}
	y += 12;
	put_section(data, data->editor, &y, "PROPERTIES -----");
	i = 8;
	while (i < 10)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}
	draw_hline(data, data->editor, y + 4);
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 8.0f);
	mlx_string_put(data->mlx, data->editor, PANEL_PAD, y + 4,
		(mlx_color){.rgba = COL_FOOTER}, "fgargot && mabarrer | miniRT");
}

void	print_hit_info(t_data *data, t_hit_record hit, double mouse_x,
		double mouse_y)
{
	int	y;
	int	panel_h;

	init_editor(data);
	mlx_clear_window(data->mlx, data->editor, (mlx_color){.rgba = COL_WHITE});
	if (!hit.object)
		return ;
	mlx_set_window_size(data->mlx, data->editor, EDITOR_W, EDITOR_H);
	panel_h = TITLE_H + LINE_H * 25 + 60;
	mlx_set_font(data->mlx, "resources/font.ttf");
	// bg + title + border
	fill_rect(data, (t_vec2){PANEL_X, PANEL_Y}, (t_vec2){PANEL_W, panel_h},
		(mlx_color){.rgba = COL_BG});
	fill_rect(data, (t_vec2){PANEL_X, PANEL_Y}, (t_vec2){PANEL_W, TITLE_H},
		(mlx_color){.rgba = COL_TITLEBAR});
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 14.0f);
	mlx_string_put(data->mlx, data->editor, PANEL_X + PANEL_PAD, PANEL_Y + 16,
		(mlx_color){.rgba = COL_WHITE}, "miniRT INSPECTOR |      ;)");
	y = PANEL_Y + TITLE_H + 8;
	// object section
	put_section(data, data->editor, &y, "OBJECT ----");
	put_row(data, data->editor, &y, "addr", (mlx_color){.rgba = COL_ADDR}, "%p",
		hit.object);
	put_row(data, data->editor, &y, "type", (mlx_color){.rgba = COL_TYPE}, "%s",
		obj_type_name(hit.object->type));
	// hit section
	put_section(data, data->editor, &y, "HIT ------");
	put_row(data, data->editor, &y, "point", (mlx_color){.rgba = COL_VALUE},
		"%.3f  %.3f  %.3f", hit.point.x, hit.point.y, hit.point.z);
	put_row(data, data->editor, &y, "mouse", (mlx_color){.rgba = COL_VALUE},
		"u=%.1f  v=%.1f", mouse_x, mouse_y);
	// surface
	put_section(data, data->editor, &y, "SURFACE --");
	put_row(data, data->editor, &y, "normal", (mlx_color){.rgba = COL_VALUE},
		"%.3f  %.3f  %.3f", hit.normal.x, hit.normal.y, hit.normal.z);
	put_row(data, data->editor, &y, "color", (mlx_color){.rgba = COL_VALUE},
		"%.3f  %.3f  %.3f", hit.color.x, hit.color.y, hit.color.z);
	setup_sliders(data);
	draw_editor(data);
}
