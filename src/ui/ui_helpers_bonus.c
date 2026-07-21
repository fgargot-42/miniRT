/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_helpers_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 19:03:45 by mabarrer          #+#    #+#             */
/*   Updated: 2026/07/21 19:20:10 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "ui.h"
#include "object.h"
#include "miniRT.h"
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
