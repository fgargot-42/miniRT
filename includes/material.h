/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:27:33 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/25 00:41:28 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "veclib.h"
# include "mlx.h"
# include "libft.h"
# include "parser_bonus.h"

typedef union u_uv_texture
{
	struct
	{
		t_vec2	tex_a;
		t_vec2	tex_b;
		t_vec2	tex_c;
	};
	t_vec2	origin;
}	t_uv_texture;

typedef struct s_texture
{
	int			width;
	int			height;
	mlx_image	data;
	mlx_context	mlx;
}	t_texture;

typedef struct s_material
{
	char		*name;
	t_vec3		amb_color;
	t_vec3		diff_color;
	t_vec3		spec_color;
	t_vec3		emissive;
	t_texture	*color_tex;
	t_texture	*normal_tex;
	t_texture	*spec_tex;
	t_texture	*mask_tex;
	double		density;
	double		specular;
	double		opacity;
}	t_material;

int			parse_new_material(char *line, t_array *materials,
				void *mat);
typedef int	(*t_obj_parser_fc)(char *, t_material *, int);
int			parse_mat_exponent(char *line, t_material *mat, int line_nb);
int			parse_mat_ambient(char *line, t_material *mat, int line_nb);
int			parse_mat_diffuse(char *line, t_material *mat, int line_nb);
int			parse_mat_specular(char *line, t_material *mat, int line_nb);
int			parse_mat_emissive(char *line, t_material *mat, int line_nb);
int			parse_mat_density(char *line, t_material *mat, int line_nb);
int			parse_mat_opacity(char *line, t_material *mat, int line_nb);
int			parse_mat_tex(char *line, t_texture **tex, t_parser_ctx *ctx,
				char *param);
void		destroy_material(void *o);
void		destroy_texture(t_texture **t);

#endif // MATERIAL_H
