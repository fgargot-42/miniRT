/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:48:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/19 21:32:12 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"
#include "math.h"

static t_hit_fn	get_hit_fn(t_obj_type type)
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

int	hit_list(t_array obj, t_ray *ray, double *closest,
	t_hit_record *rec)
{
	size_t			i;
	int				hit_curr;
	int				hit;
	t_hit_record	temp;
	t_hit_fn		hit_fn;

	hit = 0;
	i = 0;
	while (i < obj.len)
	{
		hit_curr = 0;
		hit_fn = get_hit_fn(((t_object *)obj.array[i])->type);
		if (hit_fn)
			hit_curr = hit_fn((t_object *)obj.array[i], ray, *closest, &temp);
		if (hit_curr && temp.t >= T_MIN && temp.t < *closest)
		{
			hit = 1;
			*closest = temp.t;
			*rec = temp;
		}
		i++;
	}
	return (hit);
}

int	hit_scene(t_scene *scene, t_ray *ray, double t_max, t_hit_record *rec)
{
	int		hit[2];
	double	closest;

	hit[0] = 0;
	hit[1] = 0;
	closest = t_max;
	ray->inv_direction = (t_vec3){{1 / ray->direction.x, 1 / ray->direction.y,
		1 / ray->direction.z}};
	hit[0] = hit_list(scene->objects, ray, &closest, rec);
	hit[1] = hit_bvh(scene, ray, &closest, rec);
	return (hit[0] || hit[1]);
}
