/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 23:39:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 22:52:41 by fgargot          ###   ########.fr       */
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

t_hit_fn	get_hit_fn(t_obj_type type)
{
	static t_hit_fn		hit_list[] = {
	[OBJ_PLANE] = hit_plane,
	[OBJ_SPHERE] = hit_sphere,
	[OBJ_CYLINDER] = hit_cylinder};

	if (type < OBJ_PLANE || type > OBJ_CYLINDER)
		return (NULL);
	return (hit_list[type]);
}
