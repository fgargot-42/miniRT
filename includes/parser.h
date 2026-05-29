/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:50:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/29 23:54:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "miniRT.h"

// PARSING

typedef t_object	*(*t_parser_func)(char **, t_parser_ctx *, void *);
int					parse_scene(char *file, t_data *data);
t_object			*parse_ambient(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_camera(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_sky(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_light(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_sphere(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_plane(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_cylinder(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_cone(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_hyperboloid(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_paraboloid(char **line_split, t_parser_ctx *ctx,
						void *mlx);
t_object			*parse_triangle(char **line_split, t_parser_ctx *ctx,
						void *mlx);
int 				parse_skybox(char *line, t_data *data, t_parser_ctx *ctx);

char				**parse_vector_split(char *param, char *object, int line_nb);
int					add_element_to_scene(t_scene *scene, t_parser_ctx *ctx);
void				print_parse_error(char *message, char *element,
						int line_nb);
void				clear_gnl(int fd, char *line);
int					check_array_size(char **array, int expected, char *object,
						int line_nb);
int					parse_vector(char *param, t_vec3 *v_res, char *object,
						int line_nb);
int					parse_double(char *param, double *res, char *object,
						int line_nb);
int					parse_texture_file(char *param, t_object *obj,
						t_parser_ctx *ctx, void *mlx);
#endif
