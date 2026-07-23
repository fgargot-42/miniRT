/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:50:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/24 00:48:04 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "libft.h"
# include "parser.h"

t_object	*parse_sky(char **line_split, t_parser_ctx *ctx);
t_object	*parse_cone(char **line_split, t_parser_ctx *ctx);
t_object	*parse_hyperboloid(char **line_split, t_parser_ctx *ctx);
t_object	*parse_paraboloid(char **line_split, t_parser_ctx *ctx);
t_object	*parse_triangle(char **line_split, t_parser_ctx *ctx);
int			parse_skybox(char *line, t_parser_ctx *ctx);

int			parse_texture_file(char *param, t_object *obj, t_parser_ctx *ctx);
int			parse_spec_texture_file(char *param, t_object *obj,
				t_parser_ctx *ctx);
int			parse_bump_texture_file(char *param, t_object *obj,
				t_parser_ctx *ctx);
void		add_triangles_to_scene(t_scene *scene, t_array triangles);
void		add_materials_to_scene(t_scene *scene, t_array materials);

#endif // PARSER_BONUS_H
