/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 23:51:36 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/03 02:00:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "veclib.h"
# include "matlib.h"
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
		t_mat3	transform_axis;
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
	int			width;
	int			height;
	mlx_image	data;
	mlx_context	mlx;
}	t_texture;

typedef struct s_material
{
	char		*name;
	t_vec3		amb_color;
	t_vec3		diff_color;
	t_vec3		spec_color;
	t_vec3		emissive;
	t_texture	*color_tex;
	t_texture	*normal_tex;
	t_texture	*spec_tex;
	double		density;
	double		specular;
	double		opacity;
}	t_material;

typedef struct s_object
{
	t_obj_type		type;
	t_obj_prop		props;
	t_vec3			position;
	t_vec3			direction; // normal for planes
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

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

typedef struct s_bvh
{
	t_aabb			aabb;
	struct s_bvh	*left;
	struct s_bvh	*right;
	t_array			objects;
	int				first_index;
	int				nb_elements;
	int				depth;
}	t_bvh;

typedef struct s_sah
{
	double	cost;
	int		axis;
	double	pos;
	int		count_l;
	int		count_r;
}	t_sah;

typedef struct s_scene
{
	t_array		objects;
	t_array		bvh_objects;
	t_array		lights;
	t_object	*ambient;
	t_object	*sky;
	t_texture	*skybox;
	t_object	*cam;
	t_object	*selected;
	t_array		mat;
	t_bvh		*bvh;
	int			bvh_display_level;
}	t_scene;

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

typedef struct s_bvh_hit_ctx
{
	t_scene			*scene;
	t_ray			*ray;
	double			*closest;
	t_hit_record	*rec;
}	t_bvh_hit_ctx;

typedef struct s_bvh_state
{
	t_bvh	*stack[2 * BVH_DEPTH];
	t_bvh	*node;
	t_bvh	*child[2];
	double	dist[2];
	int		depth;
}	t_bvh_state;

typedef int	(*t_hit_fn)(t_object *, t_ray *, double, t_hit_record *);

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
	// temp slider -> ui.h
	t_slider	sliders[MAX_SLIDERS];
	int			nb_sliders;
	int			dragging_slider;
}	t_data;

typedef struct s_parser_ctx
{
	int			fd;
	void		*mlx;
	t_object	*obj;
	t_material	*current_mat;
	t_material	*mat_parse;
	int			line_nb;
	char		*rt_path;
}	t_parser_ctx;

typedef struct s_row_info
{
	mlx_window	win;
	int			*y;
	const char	*label;
	mlx_color	val_col;
}	t_row_info;

typedef struct s_bin
{
	t_aabb	aabb;
	int		count;
}	t_bin;

#endif //TYPES_H
