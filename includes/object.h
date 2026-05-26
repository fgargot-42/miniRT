/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:00:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/26 19:46:39 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "veclib.h"
# include "material.h"
# include "miniRT.h"

typedef struct s_object_model
{
	t_vec3		position;
	t_list		*vertex_list;
	t_list		*normal_list;
	t_list		*texture_uv_list;
	t_list		*materials;
	t_list		*triangles;
	t_texture	*tex;
}	t_object_model;

int		parse_obj_file(char *rt_file, char *file, t_data *data, int line_nb);
void	update_box_min_max(t_object *obj);
int		parse_face(char *line, t_object_model *model, t_material *mat,
			int line_nb);
int		parse_vertex(char *line, t_list **vertex_list, int line_nb);
int		parse_normal(char *line, t_list **normal_list, int line_nb);
int		parse_texture(char *line, t_list **texture_list, int line_nb);
int		import_materials(char *mtl_file, t_list **mat_list, char *rt_path);

#endif  // OBJECT_H
