/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:00:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/08 22:46:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "veclib.h"
# include "parser.h"
# include "material.h"
# include "miniRT.h"

typedef struct s_object_model
{
	t_vec3		position;
	t_vec3		**vertices;
	t_vec2		**vertex_uv;
	t_vec3		**vertex_normals;
	t_array		materials;
	t_array		triangles;
	t_texture	*tex;
}	t_object_model;

int		parse_obj_file(char *file, t_data *data, t_parser_ctx *ctx);
void	update_box_min_max(t_object *obj);
int		parse_face(char *line, t_object_model *model, t_material *mat,
			int line_nb);
int		parse_vertex(char *line, t_vec3 ***vertices, int line_nb);
int		parse_normal(char *line, t_vec3 ***normal_array, int line_nb);
int		parse_texture(char *line, t_vec2 ***texture_array, int line_nb);
int		import_materials(char *mtl_file, t_array *materials, char *obj_path,
			mlx_context	mlx);
int		parse_obj_tex_file(t_object_model *obj, char *rt_path,
			char *tex_file, void *mlx);

#endif  // OBJECT_H
