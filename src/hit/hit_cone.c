/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:34:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/15 23:37:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cone *cone,
	t_vec3 v_hit)
{
	t_vec3	normal;
	t_vec3	oc;
	double	z_cap;

	z_cap = 2 * (v_hit.z > 0) - 1;
	oc = vec_apply_rotation_z(vec_sub(ray->origin, cone->center),
		cone->transform_axis);
	normal = vec_normalize(vec_reverse_rotation(
				(t_vec3){v_hit.x, v_hit.y, -z_cap * cone->tan_angle}, cone->transform_axis));
	if (near_equal(v_hit.z, cone->height, 1e-6) || near_equal(v_hit.z, -cone->depth, 1e-6))
		normal = vec_reverse_rotation((t_vec3){0, 0, z_cap}, cone->transform_axis);
	rec->t = vec_distance(v_hit, oc);
	rec->point = ray_at(*ray, rec->t);
	rec->normal = normal;
	rec->color = cone->color;
	rec->object.cone = cone;
}

static int	get_intersections(double *roots, t_vec3 *v_hit, t_ray *ray,
	t_cone *cone)
{
	t_vec3	oc;
	t_vec3	rd;
	double	square_tan;
	int		nb_roots;

	oc = vec_sub(ray->origin, cone->center);
	rd = ray->direction;
	square_tan = cone->tan_angle * cone->tan_angle;
	if (!near_equal(cone->axis.z, 1, 1e-6))
	{
		oc = vec_apply_rotation_z(oc, cone->transform_axis);
		rd = vec_apply_rotation_z(rd, cone->transform_axis);
	}
	nb_roots = get_polynom2_roots(roots,
			rd.x * rd.x + rd.y * rd.y - rd.z * rd.z * square_tan,
			2.0 * (oc.x * rd.x + oc.y * rd.y - oc.z * rd.z * square_tan),
			oc.x * oc.x + oc.y * oc.y - oc.z * oc.z * square_tan);
	if (nb_roots == 0)
		return (0);
	v_hit[0] = vec_add(oc, vec_scale(rd, roots[0]));
	v_hit[1] = vec_add(oc, vec_scale(rd, roots[1]));
	return (nb_roots);
}

static void	update_cap_hit(t_cone *cone, t_ray *ray, t_vec3 *v_hit,
	double *roots)
{
	double	cap_z;
	double	z_scale;
	t_vec3	oc;
	t_vec3	rd;

	oc = vec_sub(ray->origin, cone->center);
	rd = ray->direction;
	cap_z = 0;
	if (!near_equal(cone->axis.z, 1, 1e-6))
	{
		oc = vec_apply_rotation_z(oc, cone->transform_axis);
		rd = vec_apply_rotation_z(rd, cone->transform_axis);
	}
	if ((oc.z > cone->height && v_hit[0].z > cone->height)
		|| (oc.z > -cone->depth && oc.z < cone->height && rd.z > 0))
		cap_z = cone->height;
	if ((oc.z < -cone->depth && v_hit[0].z < -cone->depth)
		|| (oc.z > -cone->depth && oc.z < cone->height && rd.z < 0))
		cap_z = -cone->depth;
	z_scale = (cap_z - oc.z) / (v_hit[1].z - oc.z);
	v_hit[1] = vec_add(vec_scale(vec_sub(v_hit[1], oc), z_scale), oc); 
	roots[1] = (roots[1] - fmax(0, roots[0])) * z_scale + fmax(0, roots[0]);
}

static int	hit_cone_cap(t_cone *cone, t_ray *ray, t_vec3 *v_hit, double *roots)
{
	t_vec3	oc;

	oc = vec_sub(ray->origin, cone->center);
	if (!near_equal(cone->axis.z, 1, 1e-6))
		oc = vec_apply_rotation_z(oc, cone->transform_axis);
	if (v_hit[1].z > cone->height && v_hit[0].z > cone->height)
		return (0);
	if (v_hit[1].z < -cone->depth && v_hit[0].z < -cone->depth)
		return (0);
	if (oc.z >=0 && oc.z < cone->height && v_hit[1].z < cone->height
		&& vec_length((t_vec3){oc.x, oc.y, 0}) < oc.z * cone->tan_angle)
		return (1);
	if (oc.z <= 0 && oc.z > -cone->depth && v_hit[1].z > -cone->depth
		&& vec_length((t_vec3){oc.x, oc.y, 0}) < -oc.z * cone->tan_angle)
		return (1);
	update_cap_hit(cone, ray, v_hit, roots);
	if (near_equal(v_hit[1].z, cone->height, 1e-6) && cone->height * cone->tan_angle >= vec_length((t_vec3){v_hit[1].x, v_hit[1].y, 0}))
		return (1);
	if (near_equal(v_hit[1].z, -cone->depth, 1e-6) && cone->depth * cone->tan_angle >= vec_length((t_vec3){v_hit[1].x, v_hit[1].y, 0}))
		return (1);
	return (0);
}

int	hit_cone(t_cone *cone, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	roots[2];
	t_vec3	v_hit[2];
	int		nb_hits;

	nb_hits = get_intersections(roots, v_hit, ray, cone);
	if (!nb_hits || roots[0] > t_max)
		return (0);
	if (roots[0] >= T_MIN && v_hit[0].z <= cone->height
		&& v_hit[0].z >= -cone->depth)
	{
		update_hit_record(rec, ray, cone, v_hit[0]);
		return (1);
	}
	if (nb_hits == 1)
		return (0);
	nb_hits = hit_cone_cap(cone, ray, v_hit, roots);
	if (nb_hits && roots[1] >= T_MIN && roots[1] <= t_max)
	{
		update_hit_record(rec, ray, cone, v_hit[1]);
		return (1);
	}
	return (0);
}
