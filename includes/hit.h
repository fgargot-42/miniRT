/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 16:30:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/24 00:00:59 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HIT_H
# define HIT_H

# include "veclib.h"
# include "bvh.h"

typedef enum e_obj_type	t_obj_type;
typedef struct s_object	t_object;
typedef struct s_scene	t_scene;

typedef struct s_ray
{
	t_vec3		origin;
	t_vec3		direction;
	t_vec3		inv_direction;
	double		refraction;
}	t_ray;

typedef struct s_hit_record
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	*object;
	double		specular;
	double		shininess;
	int			depth;
}	t_hit_record;

typedef int				(*t_hit_fn)(t_object *, t_ray *, double,
							t_hit_record *);

typedef struct s_hit_ctx
{
	double	t_max;
	union
	{
		struct
		{
			t_vec3	oc;
			t_vec3	rd;
		};
		t_ray	obj_ray;
	};
	double	render_t;
	t_vec3	render_hit;
}	t_hit_ctx;

t_vec3				face_normal(t_ray *ray, t_vec3 inverted);
t_hit_fn			get_hit_fn(t_obj_type type);
int					hit_list(t_array obj, t_ray *ray, double *closest,
						t_hit_record *rec);
int					hit_scene(t_scene *scene, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_sphere(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_plane(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_cylinder(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
double				hit_bvh_box(t_bvh *bvh, t_ray *ray, double dist);
int					hit_bvh(t_scene *scene, t_ray *ray, double *closest,
						t_hit_record *rec);
int					hit_object_in_bvh(t_bvh *bvh, t_ray *ray, double *closest,
						t_hit_record *rec);
int					draw_box_bounds(t_bvh_hit_ctx *ctx, t_bvh_state state);

t_vec3				ray_at(t_ray ray, double t);
t_ray				get_object_relative_ray(t_ray ray, t_object *obj);

#endif	//HIT_H
