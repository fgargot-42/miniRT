/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:34:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/09 18:32:04 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static double	vec_get_intersection(t_vec3 a_origin, t_vec3 a_dir,
	t_vec3 b_dir, t_vec3 *intersection)
{
	double	cross;
	double	t;

	cross = a_dir.x * b_dir.z - a_dir.z * b_dir.x;
	if (cross == 0)
		return (0);
	t = (a_origin.x * b_dir.z - a_origin.z * b_dir.x) / cross;
	*intersection = (t_vec3){t * a_dir.x, t * a_dir.y, t * a_dir.z};
	return (t);
}

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cone *cone,
	t_vec3 v_hit)
{
	t_vec3	normal;
	t_vec3	oc;

	oc = vec_apply_rotation_z(vec_sub(ray->origin, cyl->center),
		cyl->transform_axis);
	if ((fabs(v_hit.z) - cyl->height / 2.0) < 1e-6)
		normal = vec_reverse_rotation(
				(t_vec3){0, 0, 2 * (v_hit.z > 0) - 1}, cyl->transform_axis);
	else
		normal = vec_normalize(vec_reverse_rotation(
				(t_vec3){v_hit.x, v_hit.y, 0}, cyl->transform_axis));
	rec->t = vec_distance(v_hit, oc);
	rec->point = ray_at(*ray, rec->t);
	rec->normal = normal;
	rec->color = cyl->color;
	rec->object.cylinder = cyl;
}

static int	get_intersections(double *roots, t_vec3 *v_hit, t_ray *ray,
	t_cone *cone)
{
	t_vec3	oc;
	t_vec3	rd;
	t_vec3	v_cone[2];
	int		nb_hits;

	nb_hits = 0;
	oc = vec_sub(ray->origin, cone->center);
	oc = vec_apply_rotation_z(oc, cone->transform_axis);
	rd = vec_apply_rotation_z(ray->direction, cone->transform_axis);
	v_cone[0] = vec_normalize((t_vec3){cone->tan_angle, 0, 1});
	v_cone[1] = vec_normalize((t_vec3){-cone->tan_angle, 0, 1});
	if (fabs(rd.x * v_cone[0].z - rd.z * v_cone[0].x) > 1e-6)
	{
		roots[0] = vec_get_intersection(oc, rd, v_cone[0], &v_hit[0]);
		//if (v_hit[0].z >= -cone->depth && v_hit[0].z <= cone->height
		//	&& vec_length((t_vec){v_hit[0].x, v_hit[0].y, 0}) - 	
		nb_hits++;
	}
	if (fabs(rd.x * v_cone[1].z - rd.z * v_cone[1].x) > 1e-6)
	{
		roots[1] = vec_get_intersection(oc, rd, v_cone[1], &v_hit[1]);
		nb_hits++;
	}
	return (nb_hits);
}

static int	hit_cylinder_cap(t_cylinder *cyl, t_ray *ray, t_vec3 *v_hit,
	double *roots)
{
	t_vec3	oc;
	t_vec3	rd;
	double	cap_z;
	double	z_scale;

	oc = vec_sub(ray->origin, cyl->center);
	oc = vec_apply_rotation_z(oc, cyl->transform_axis);
	rd = vec_apply_rotation_z(ray->direction, cyl->transform_axis);
	if (v_hit[1].z > cyl->height / 2.0 && v_hit[0].z > cyl->height / 2.0)
		return (0);
	if (v_hit[1].z < -cyl->height / 2.0 && v_hit[0].z < -cyl->height / 2.0)
		return (0);
	if (fabs(oc.z) < cyl->height / 2.0 && fabs(v_hit[1].z) < cyl->height / 2.0)
		return (1);
	if (fabs(oc.z) - cyl->height / 2.0 < 1e-6)
	{
		cap_z = (2 * (rd.z > 0) - 1) * cyl->height / 2.0;
		z_scale = (cap_z - oc.z) / (v_hit[1].z - oc.z);
		v_hit[1] = vec_add(vec_scale(vec_sub(v_hit[1], oc), z_scale), oc); 
		roots[1] *= z_scale;
	}
	else
	{
		cap_z = (2 * (v_hit[0].z > 0) - 1) * cyl->height / 2.0;
		z_scale = (cap_z - v_hit[0].z) / (v_hit[1].z - v_hit[0].z);
		v_hit[1] = vec_add(vec_scale(vec_sub(v_hit[1], v_hit[0]), z_scale), v_hit[0]);
		roots[1] = (roots[1] - roots[0]) * z_scale + roots[0];
	}
	return (1);
}

int	hit_cone(t_cone *cone, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	roots[2];
	t_vec3	v_hit[2];
	int		nb_hits;

	nb_hits = get_intersections(roots, v_hit, ray, cone);
	if (!nb_hits || roots[0] > t_max)
		return (0);
	if (roots[0] >= T_MIN && fabs(v_hit[0].z) <= cyl->height / 2.0)
	{
		update_hit_record(rec, ray, cyl, v_hit[0]);
		return (1);
	}
	has_intersections = hit_cylinder_cap(cyl, ray, v_hit, roots);
	if (has_intersections && roots[1] >= T_MIN && roots[1] <= t_max)
	{
		update_hit_record(rec, ray, cyl, v_hit[1]);
		return (1);
	}
	return (0);
}
