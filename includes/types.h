/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 23:51:36 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/29 23:57:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "veclib.h"
# include "libft.h"
# include "ui.h"

typedef struct s_ray
{
	t_vec3		origin;
	t_vec3		direction;
	t_vec3		inv_direction;
}	t_ray;

typedef enum e_obj_type
{
	OBJ_AMBIENT,
	OBJ_CAMERA,
	OBJ_SKY,
	OBJ_LIGHT,
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE,
	OBJ_HYPERBOLOID,
	OBJ_PARABOLOID,
	OBJ_TRIANGLE,
}	t_obj_type;

typedef union u_obj_prop
{
	// cylinder, cone, hyper/paraboloid
	struct
	{
		double	tan_angle;
		double	height;
		double	depth;
		double	**transform_axis;
	};
	// camera
	struct
	{
		double	fov;
		double	pitch;
		double	yaw;
	};
	struct
	{
		t_vec3	a;
		t_vec3	b;
		t_vec3	c;
	};
	double	intensity; // light/ambient
}	t_obj_prop;

typedef union u_uv_texture
{
	struct
	{
		t_vec2	tex_a;
		t_vec2	tex_b;
		t_vec2	tex_c;
	};
	t_vec2	origin;
}	t_uv_texture;

typedef struct s_texture
{
	int		width;
	int		height;
	void	*data;
}	t_texture;

typedef struct s_material
{
	char		*name;
	t_vec3		ambient;
	t_vec3		diffuse;
	t_vec3		specular;
	t_vec3		emissive;
	t_texture	*color_tex;
	double		density;
	double		shininess;
	double		opacity;
}	t_material;

typedef struct s_object
{
	t_obj_type		type;
	t_obj_prop		props;
	t_vec3			position;
	t_vec3			direction; // normal for planes, default {0, 0, 0} for spheres
	t_vec3			scale;
	t_vec3			color;
	int				checker;
	double			radius;
	double			angle;
	double			specular;
	double			shininess;
	t_uv_texture	uv;
	t_material		*material;
	t_texture		*tex;
}	t_object;

typedef struct s_hit_record
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	*object;
	double		specular;
	double		shininess;
}	t_hit_record;

typedef struct s_hit_fn
{
	t_obj_type	type;
	int			(*hit_fn)(t_object *, t_ray *, double, t_hit_record *);
}	t_hit_fn;

typedef struct s_bvh
{
	t_vec3			aabb_min;
	t_vec3			aabb_max;
	struct s_bvh	*left;
	struct s_bvh	*right;
	t_object		**objects;
	int				first_index;
	int				nb_elements;
	int				depth;
}	t_bvh;

typedef struct s_scene
{
	t_list		*objects;
	t_list		*lights;
	t_object	*ambient;
	t_object	*sky;
	t_texture	*skybox;
	t_object	*cam;
	t_object	*selected;
	t_bvh		*bvh;
}	t_scene;

typedef struct s_data
{
	mlx_context	mlx;
	mlx_window	win;
	mlx_window	editor;
	mlx_image	img;
	t_scene		*scene;
	int			r_click_hold;
	int			w_click_hold;
	int			last_mouse_x;
	int			last_mouse_y;
	int			render_scale;
	int			th_nb;
	int			nb_threads;
	int			bvh_display_depth;
	// temp slider -> ui.h
	t_slider	sliders[MAX_SLIDERS];
	int			nb_sliders;
	int			dragging_slider;
}	t_data;

typedef struct	s_parser_ctx
{
	int			fd;
	void		*mlx;
	t_object	*obj;
	int			line_nb;
	char		*rt_path;
}	t_parser_ctx;

#endif //TYPES_H
