/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:00:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 01:09:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "veclib.h"
# include "parser_bonus.h"
# include "material.h"
# include "miniRT_bonus.h"

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

void	init_object_model(t_object_model *obj);
void	destroy_object_model(t_object_model *obj);

int		parse_obj_file(char *file, t_data *data, t_parser_ctx *ctx);
void	update_box_min_max(t_object *obj);
bool	parse_face(char *line, t_object_model *model, t_material *mat,
			int line_nb);
bool	parse_vertex(char *line, t_array *vertices, int line_nb);
bool	parse_normal(char *line, t_array *normal_array, int line_nb);
bool	parse_texture(char *line, t_array *texture_array, int line_nb);
int		import_materials(char *mtl_file, t_array *materials, char *obj_path,
			mlx_context	mlx);
int		parse_obj_tex_file(t_object_model *obj, char *rt_path,
			char *tex_file, void *mlx);

#endif  // OBJECT_H
