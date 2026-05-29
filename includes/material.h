/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:27:33 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/29 23:54:41 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "miniRT.h"
# include "veclib.h"
# include "libft.h"
# include "parser.h"

int			parse_new_material(char *line, t_list **mat_list,
				t_material **mat);
typedef int	(*t_obj_parser_fc)(char *, t_material *, int);
int			parse_mat_exponent(char *line, t_material *mat, int line_nb);
int			parse_mat_ambient(char *line, t_material *mat, int line_nb);
int			parse_mat_diffuse(char *line, t_material *mat, int line_nb);
int			parse_mat_specular(char *line, t_material *mat, int line_nb);
int			parse_mat_emissive(char *line, t_material *mat, int line_nb);
int			parse_mat_density(char *line, t_material *mat, int line_nb);
int			parse_mat_opacity(char *line, t_material *mat, int line_nb);
int			parse_mat_color_tex(char *line, t_material *mat, t_parser_ctx *ctx);

#endif // MATERIAL_H
