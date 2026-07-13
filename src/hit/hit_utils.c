/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 23:39:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/13 21:11:07 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

t_vec3	face_normal(t_ray *ray, t_vec3 inverted)
{
	if (vec3_dot(ray->direction, inverted) > 0)
		return (vec3_scale(inverted, -1.0));
	return (inverted);
}

void	apply_checker(t_hit_record *rec, t_object *obj, t_vec3 point)
{
	int		x;
	int		y;
	int		z;

	x = (int)floor(point.x + 1e-8);
	y = (int)floor(point.y + 1e-8);
	z = (int)floor(point.z + 1e-8);
	if ((x + y + z) % 2 == 0)
		rec->color = obj->checker_color;
	else
		rec->color = obj->color;
}
