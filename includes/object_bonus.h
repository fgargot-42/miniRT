/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:00:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 00:13:26 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_BONUS_H
# define OBJECT_BONUS_H

# include "material.h"
# include "matlib.h"

typedef struct s_data	t_data;

typedef struct s_object_model
{
	t_vec3		position;
	t_array		vertices;
	t_array		vertex_uv;
	t_array		vertex_normals;
	t_array		materials;
	t_array		triangles;
	t_texture	*tex;
}	t_object_model;

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
	struct
	{
		t_vec3	a;
		t_vec3	b;
		t_vec3	c;
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
	t_vec3			checker_color;
	double			checker_scale;
	t_uv_texture	uv;
	t_material		*material;
}	t_object;

t_object	*create_object(void *object, t_obj_type type);
void		init_object_model(t_object_model *obj);
void		destroy_object_model(t_object_model *obj);

int			parse_obj_file(char *file, t_data *data, t_parser_ctx *ctx);
void		update_box_min_max(t_object *obj);
bool		parse_face(char *line, t_object_model *model, t_material *mat,
				int line_nb);
bool		parse_vertex(char *line, t_array *vertices, int line_nb);
bool		parse_normal(char *line, t_array *normal_array, int line_nb);
bool		parse_texture(char *line, t_array *texture_array, int line_nb);
int			import_materials(char *mtl_file, t_array *materials, char *obj_path,
				mlx_context	mlx);
int			parse_obj_tex_file(t_object_model *obj, char *rt_path,
				char *tex_file, void *mlx);

#endif  // OBJECT_BONUS_H
