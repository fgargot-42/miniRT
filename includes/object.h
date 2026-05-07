/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:00:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/08 00:55:38 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "veclib.h"
# include "material.h"
# include "miniRT.h"

typedef	struct s_object_model
{
	t_list	*vertex_list;
	t_list	*normal_list;
	t_list	*texture_list;
	t_list	*materials;
	t_list	*triangles;
}	t_object_model;

#endif  // OBJECT_H

int		parse_obj_file(char *rt_file, char *file, t_scene *scene);
int		parse_face(char *line, t_object_model *model, t_material *mat, int line_nb);
int		parse_vertex(char *line, t_list **vertex_list, int line_nb);
int		parse_normal(char *line, t_list **normal_list, int line_nb);
int		parse_texture(char *line, t_list **texture_list, int line_nb);
