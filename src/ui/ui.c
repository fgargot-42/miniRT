/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 00:42:26 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 18:43:47 by mabarrer         ###   ########.fr       */
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

static const char	*g_obj_names[] = {"AMBIENT", "CAMERA", "LIGHT",
	"PLANE", "SPHERE", "CYLINDER"};

static const char	*obj_type_name(t_object *o)
{
	long	max;

	if (!o)
		return ("UNKNOWN");
	max = sizeof(g_obj_names) / sizeof(*g_obj_names);
	if (o->type < 0 || o->type >= max)
		return ("UNKNOWN");
	return (g_obj_names[o->type]);
}

void	setup_sliders(t_data *data)
{
	t_object	*obj;

	data->nb_sliders = 0;
	obj = data->scene->selected;
	if (!obj)
		return ;
	setup_transform_sliders(data, obj);
	setup_color_sliders(data, obj);
	setup_property_sliders(data, obj);
	setup_ambient_sliders(data, obj, 11);
	data->nb_sliders = 15;
}

/*
   PANEL_PAD           SLD_X          SLD_X+SLD_W    SLD_X+SLD_W+50
   label               [====filled====|----empty----]    value
   ^ curseur
 */

static double	slider_ratio(t_slider *s)
{
	double	t;

	if (!s->value || s->max == s->min)
		return (0.0);
	t = (*s->value - s->min) / (s->max - s->min);
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	return (t);
}

static mlx_color	slider_dim(t_slider *s)
{
	mlx_color	c;

	c.r = s->col.r / 5;
	c.g = s->col.g / 5;
	c.b = s->col.b / 5;
	c.a = 255;
	return (c);
}

static void	draw_slider(t_data *data, t_slider *s)
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

static void	draw_slider_group(t_data *d, int start, int end, int *y)
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

static void	draw_title(t_data *d, int *y, char *title)
{
	put_section(d, d->editor, y, title);
}

static void	draw_group(t_data *d, t_vec2 range, int *y, char *title)
{
	draw_title(d, y, title);
	draw_slider_group(d, range.x, range.y, y);
	*y += 12;
}

static void	draw_ambient(t_data *d, int *y)
{
	draw_title(d, y, "AMBIENT -----");
	draw_slider_group(d, 11, 15, y);
}

void	draw_editor(t_data *d, double mx, double my)
{
	int	y;

	if (!d->editor || d->nb_sliders == 0)
		return ;
	mlx_clear_window(d->mlx, d->editor, (mlx_color){.rgba = COL_WHITE});
	fill_rect(d, (t_vec2){{0, 0}}, (t_vec2){{EDITOR_W, EDITOR_H}},
		(mlx_color){.rgba = COL_BG});
	header(d, mx, my);
	y = SLD_BASE_Y - 130;
	draw_group(d, (t_vec2){{0, 3}}, &y, "TRANSFORM -");
	draw_group(d, (t_vec2){{3, 6}}, &y, "ROTATION -");
	draw_group(d, (t_vec2){{6, 9}}, &y, "COLOR -----");
	draw_group(d, (t_vec2){{9, 11}}, &y, "PROPERTIES -----");
	draw_ambient(d, &y);
	draw_hline(d, d->editor, y + 4);
	mlx_set_font_scale(d->mlx, "resources/font.ttf", 8.0f);
	mlx_string_put(d->mlx, d->editor, PANEL_PAD, y + 4,
		(mlx_color){.rgba = COL_FOOTER}, "fgargot && mabarrer | miniRT");
}

void	open_inspector(t_data *data, t_hit_record hit, double mouse_x,
		double mouse_y)
{
	int	panel_h;

	init_editor(data);
	mlx_clear_window(data->mlx, data->editor, (mlx_color){.rgba = COL_WHITE});
	if (!hit.object)
		return ;
	panel_h = TITLE_H + LINE_H * 25 + 60;
	mlx_set_font(data->mlx, "resources/font.ttf");
	fill_rect(data, (t_vec2){{PANEL_X, PANEL_Y}}, (t_vec2){{PANEL_W, panel_h}},
		(mlx_color){.rgba = COL_BG});
	fill_rect(data, (t_vec2){{PANEL_X, PANEL_Y}}, (t_vec2){{PANEL_W, TITLE_H}},
		(mlx_color){.rgba = COL_TITLEBAR});
	setup_sliders(data);
	draw_editor(data, mouse_x, mouse_y);
}
