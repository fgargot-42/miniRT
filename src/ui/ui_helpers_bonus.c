/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_helpers_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 19:03:45 by mabarrer          #+#    #+#             */
/*   Updated: 2026/08/17 18:38:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "ui.h"
#include "object_bonus.h"
#include "miniRT_bonus.h"
#include <stdarg.h>

const char	*obj_type_name(t_object *o)
{
	long				max;
	static const char	*obj_names[] = {"AMBIENT",
		"CAMERA", "SKY", "LIGHT", "PLANE",
		"SPHERE", "CYLINDER", "CONE",
		"HYPERBOLOID", "PARABOLOID", "TRIANGLE"};

	if (!o)
		return ("UNKNOWN");
	max = sizeof(obj_names) / sizeof(*obj_names);
	if (o->type < 0 || o->type >= max)
		return ("UNKNOWN");
	return (obj_names[o->type]);
}

double	slider_ratio(t_slider *s)
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

mlx_color	slider_dim(t_slider *s)
{
	mlx_color	c;

	c.r = s->col.r / 5;
	c.g = s->col.g / 5;
	c.b = s->col.b / 5;
	c.a = 255;
	return (c);
}

void	draw_button(t_data *data, t_button *button)
{
	fill_rect(data, button->pos, button->size, (mlx_color){.rgba = COL_BUTTON});
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(data->mlx, data->editor,
		button->pos.x + button->size.x / 2 - 4,
		button->pos.y + button->size.y / 2 + 4,
		(mlx_color){.rgba = COL_BTN_TEXT}, button->label);
}
