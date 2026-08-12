/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:50:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/12 02:02:48 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "veclib.h"
# include <stdbool.h>

typedef struct s_scene	t_scene;
typedef struct s_data	t_data;
typedef struct s_object	t_object;

typedef struct s_parser_ctx
{
	int			fd;
	void		*mlx;
	t_data		*data;
	t_object	*obj;
	int			line_nb;
	char		*rt_path;
	void		*current_mat;
	void		*mat_parse;
}	t_parser_ctx;

// PARSING

typedef t_object		*(*t_parser_func)(char **, t_parser_ctx *);
bool				parse_scene(char *file, t_data *data);
t_object			*parse_ambient(char **line_split, t_parser_ctx *ctx);
t_object			*parse_camera(char **line_split, t_parser_ctx *ctx);
t_object			*parse_light(char **line_split, t_parser_ctx *ctx);
t_object			*parse_sphere(char **line_split, t_parser_ctx *ctx);
t_object			*parse_plane(char **line_split, t_parser_ctx *ctx);
t_object			*parse_cylinder(char **line_split, t_parser_ctx *ctx);

char				**parse_vector_split(char *param, char *object,
						int line_nb);
int					add_specials(t_object **dst, t_parser_ctx *ctx, char *elem);
void				print_parse_error(char *message, char *element,
						int line_nb);
void				clear_gnl(int fd, char *line);
int					check_array_size(char **array, int expected, char *object,
						int line_nb);
int					parse_vector(char *param, t_vec3 *v_res, char *object,
						int line_nb);
int					parse_double(char *param, double *res, char *object,
						int line_nb);
bool				is_ignored(char *param);

#endif
