/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:49:37 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/02 18:56:39 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "hit_bonus.h"
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
	double	scale;
	
	rec->color = obj->color;
	if (!obj->checker)
		return ;
		
	scale = obj->checker_scale;
    if (scale <= 0.0)
		scale = 1.0;
	x = (int)floor(point.x / scale + 1e-8);
    y = (int)floor(point.y / scale + 1e-8);
    z = (int)floor(point.z / scale + 1e-8);
	if ((x + y + z) % 2 == 0)
		rec->color = obj->checker_color;
}

t_hit_fn	get_hit_fn(t_obj_type type)
{
	static t_hit_fn		hit_list[] = {
	[OBJ_PLANE] = hit_plane,
	[OBJ_SPHERE] = hit_sphere,
	[OBJ_CYLINDER] = hit_cylinder,
	[OBJ_CONE] = hit_cone,
	[OBJ_HYPERBOLOID] = hit_hyperboloid,
	[OBJ_PARABOLOID] = hit_paraboloid,
	[OBJ_TRIANGLE] = hit_triangle};

	if (type < OBJ_PLANE || type > OBJ_TRIANGLE)
		return (NULL);
	return (hit_list[type]);
}
