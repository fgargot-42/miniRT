/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 23:51:36 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/18 00:52:56 by fgargot          ###   ########.fr       */
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
	// camera
	struct
	{
		double	fov;
		double	pitch;
		double	yaw;
	};
	double	intensity; // light/ambient
}	t_obj_prop;

typedef struct s_object
{
	t_obj_type		type;
	t_obj_prop		props;
	t_vec3			position;
	t_vec3			direction; // normal for planes
	t_vec3			scale;
	t_vec3			color;
	double			radius;
	double			angle;
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
	t_object	*ambient;
	t_object	*cam;
	t_object	*selected;
	t_bvh		*bvh;
	int			bvh_display_level;
	t_object	*light;
}	t_scene;

typedef struct s_hit_record
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	*object;
}	t_hit_record;

typedef struct s_bvh_hit_ctx
{
	t_scene			*scene;
	t_ray			*ray;
	double			*closest;
	t_hit_record	*rec;
	int				hit;
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
