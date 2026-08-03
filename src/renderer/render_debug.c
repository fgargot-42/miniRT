/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 23:19:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 23:04:01 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <stdio.h>

void	print_fps(t_data *data, double render_time_ms, t_vec2 pos);
void	print_camera_orientation(t_data *data, t_vec2 pos);
void	print_vector(t_data *data, char *name, t_vec3 vec, t_vec2 pos);
void	print_int_value(t_data *data, char *label, int value, t_vec2 pos);

void	add_debug(t_data *data, double render_time_ms)
{
	print_fps(data, render_time_ms, (t_vec2){{10, 20}});
	print_int_value(data, "Render Scale", data->render_scale,
		(t_vec2){{10, 40}});
	print_vector(data, "POS", data->scene->cam->position, (t_vec2){{10, 60}});
	print_vector(data, "DIR", data->scene->cam->direction, (t_vec2){{10, 80}});
	print_camera_orientation(data, (t_vec2){{10, 100}});
	print_int_value(data, "BVH DISPLAY LEVEL", data->scene->bvh_display_level,
		(t_vec2){{10, 120}});
}
