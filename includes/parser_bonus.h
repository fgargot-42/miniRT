/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:50:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 20:31:15 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "libft.h"
# include "parser.h"
# include "material.h"

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

#endif // PARSER_BONUS_H
