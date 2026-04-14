/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 18:57:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/14 20:21:22 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cylinder *cyl,
	t_vec3 v_hit)
{
	t_vec3	oc;
	double	z_cap;

	z_cap = 2 * (v_hit.z > 0) - 1;
	oc = vec_sub(ray->origin, cyl->center);
	if (!near_equal(cyl->axis.z, 1, 1e-6))
		oc = vec_apply_rotation_z(oc, cyl->transform_axis);
	if (near_equal(fabs(v_hit.z), cyl->height / 2.0, 1e-6))
		rec->normal = (t_vec3){0, 0, 2 * (v_hit.z > 0) - 1};
	else
		rec->normal = vec_normalize((t_vec3){v_hit.x, v_hit.y, 0});
	if (!near_equal(cyl->axis.z, 1, 1e-6))
		rec->normal = vec_reverse_rotation(rec->normal, cyl->transform_axis);
	rec->normal = face_normal(ray, rec->normal);
	rec->t = vec_distance(v_hit, oc);
	rec->point = ray_at(*ray, rec->t);
	rec->color = cyl->color;
	rec->object.cylinder = cyl;
}

static int	get_intersections(double *roots, t_vec3 *v_hit, t_ray *ray,
	t_cylinder *cyl)
{
	t_vec3	oc;
	t_vec3	rd;
	int		nb_roots;

	oc = vec_sub(ray->origin, cyl->center);
	rd = ray->direction;
	if (!near_equal(cyl->axis.z, 1, 1e-6))
	{
		oc = vec_apply_rotation_z(oc, cyl->transform_axis);
		rd = vec_apply_rotation_z(rd, cyl->transform_axis);
	}
	nb_roots = get_polynom2_roots(roots,
			rd.x * rd.x + rd.y * rd.y,
			2.0 * (oc.x * rd.x + oc.y * rd.y),
			(oc.x * oc.x + oc.y * oc.y) - cyl->radius * cyl->radius);
	if (nb_roots == 0)
		return (0);
	v_hit[0] = vec_add(oc, vec_scale(rd, roots[0]));
	v_hit[1] = vec_add(oc, vec_scale(rd, roots[1]));
	return (1);
}

static void	update_cap_hit(t_cylinder *cyl, t_ray *ray, t_vec3 *v_hit,
	double *roots)
{
	double	cap_z;
	double	z_scale;
	double	first_root;
	t_vec3	oc;
	t_vec3	rd;

	oc = vec_sub(ray->origin, cyl->center);
	rd = ray->direction;
	first_root = 0;
	if (!near_equal(cyl->axis.z, 1, 1e-6))
	{
		oc = vec_apply_rotation_z(oc, cyl->transform_axis);
		rd = vec_apply_rotation_z(rd, cyl->transform_axis);
	}
	if ((fabs(oc.z) - cyl->height / 2.0) > 1e-6)
	{
		oc = v_hit[0];
		rd = v_hit[0];
		first_root = roots[0];
	}
	cap_z = (2 * (rd.z > 0) - 1) * cyl->height / 2.0;
	z_scale = (cap_z - oc.z) / (v_hit[1].z - oc.z);
	v_hit[1] = vec_add(vec_scale(vec_sub(v_hit[1], oc), z_scale), oc); 
	roots[1] = (roots[1] - first_root) * z_scale + first_root;
}

static int	hit_cylinder_cap(t_cylinder *cyl, t_ray *ray, t_vec3 *v_hit,
	double *roots)
{
	t_vec3	oc;

	oc = vec_sub(ray->origin, cyl->center);
	if (!near_equal(cyl->axis.z, 1, 1e-6))
		oc = vec_apply_rotation_z(oc, cyl->transform_axis);
	if (v_hit[1].z > cyl->height / 2.0 && v_hit[0].z > cyl->height / 2.0)
		return (0);
	if (v_hit[1].z < -cyl->height / 2.0 && v_hit[0].z < -cyl->height / 2.0)
		return (0);
	if (fabs(oc.z) < cyl->height / 2.0 && fabs(v_hit[1].z) < cyl->height / 2.0)
		return (1);
	update_cap_hit(cyl, ray, v_hit, roots);
	return (1);
}

int	hit_cylinder(t_cylinder *cyl, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	roots[2];
	t_vec3	v_hit[2];
	int		has_intersections;

	has_intersections = get_intersections(roots, v_hit, ray, cyl);
	if (!has_intersections || (roots[0] - t_max) > 1e-6)
		return (0);
	if (roots[0] >= T_MIN && fabs(v_hit[0].z) <= cyl->height / 2.0)
	{
		update_hit_record(rec, ray, cyl, v_hit[0]);
		return (1);
	}
	has_intersections = hit_cylinder_cap(cyl, ray, v_hit, roots);
	if (has_intersections && roots[1] >= T_MIN && (roots[1] - t_max) < 1e-6)
	{
		update_hit_record(rec, ray, cyl, v_hit[1]);
		return (1);
	}
	return (0);
}
