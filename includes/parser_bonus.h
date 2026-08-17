/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:50:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:24:17 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "libft.h"
# include "parser.h"
# include "material_bonus.h"
# include "object_bonus.h"

t_object	*parse_sky(char **line_split, t_parser_ctx *ctx);
t_object	*parse_cone(char **line_split, t_parser_ctx *ctx);
t_object	*parse_hyperboloid(char **line_split, t_parser_ctx *ctx);
t_object	*parse_paraboloid(char **line_split, t_parser_ctx *ctx);
t_object	*parse_triangle(char **line_split, t_parser_ctx *ctx);
int			parse_optional_elements(char **params, t_object *obj,
				t_parser_ctx *ctx, int i);
int			parse_skybox(char *line, t_parser_ctx *ctx);

t_material	*create_material(t_data *data);
int			parse_texture_file(char *param, t_texture **tex,
				t_parser_ctx *ctx);
void		add_triangles_to_scene(t_scene *scene, t_array triangles);
void		add_materials_to_scene(t_scene *scene, t_array materials);

bool		process_obj_file(t_object_model *obj, char *obj_file,
				t_parser_ctx *ctx);
bool		parse_obj_line(t_object_model *obj, char *line, char *obj_path,
				t_parser_ctx *ctx);
char		**rt_parser_split(char *str, char const *sep);

void		apply_illum(t_material *mat);
int			strncmp_case_i(char const *str1, char const *str2, size_t n);

#endif // PARSER_BONUS_H
