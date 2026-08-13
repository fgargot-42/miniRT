/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:00:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 00:13:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "veclib.h"
# include "matlib.h"

typedef enum e_obj_type
{
	OBJ_AMBIENT,
	OBJ_CAMERA,
	OBJ_LIGHT,
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
}	t_obj_type;

typedef union u_obj_prop
{
	// cylinder, cone, hyper/paraboloid
	struct
	{
		double	tan_angle;
		double	height;
		double	depth;
		t_mat3	transform_axis;
	};
	double	fov; // camera field of view
	double	intensity; // light/ambient
}	t_obj_prop;

typedef struct s_object
{
	t_obj_type		type;
	t_obj_prop		props;
	t_vec3			position;
	t_vec3			direction; // normal for planes
	t_vec3			rotation; // euler-angle rotation (x=pitch, y=yaw, z=roll)
	t_vec3			scale;
	t_vec3			color;
	double			radius;
	double			angle;
}	t_object;

t_object	*create_object(void *object, t_obj_type type);
t_vec3		euler_to_direction(t_vec3 euler);

#endif  // OBJECT_H
