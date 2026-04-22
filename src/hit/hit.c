/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:48:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 19:10:32 by fgargot          ###   ########.fr       */
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

static int	hit_list(t_hit_list h, t_ray *ray, double *closest,
	t_hit_record *rec)
{
	t_hit_record	temp;
	int				hit;

	hit = 0;
	while (h.list)
	{
		if (h.hit_fn(h.list->content, ray, *closest, &temp))
		{
			hit = 1;
			*closest = temp.t;
			*rec = temp;
		}
		h.list = h.list->next;
	}
	return (hit);
}

int	hit_scene(t_scene *scene, t_ray *ray, double t_max, t_hit_record *rec)
{
	int		hit;
	double	closest;

	hit = 0;
	closest = t_max;
	hit |= hit_list((t_hit_list){scene->spheres, (void *)hit_sphere},
			ray, &closest, rec);
	hit |= hit_list((t_hit_list){scene->planes, (void *)hit_plane},
			ray, &closest, rec);
	hit |= hit_list((t_hit_list){scene->cylinder, (void *)hit_cylinder},
			ray, &closest, rec);
	hit |= hit_list((t_hit_list){scene->cone, (void *)hit_cone},
			ray, &closest, rec);
	return (hit);
}
