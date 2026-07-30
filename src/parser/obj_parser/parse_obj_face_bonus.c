/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj_face_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 23:09:20 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/30 19:21:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser_bonus.h"
#include "miniRT_bonus.h"

static bool	parse_face_values(char *line_split, t_object_model *model,
	t_vec3 *vertex, t_vec2 *uv)
{
	char	**split_point;
	int		i;

	if (!model || !vertex || !uv)
		return (false);
	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (false);
	i = ft_atoi(split_point[0]);
	if (i < 1)
	{
		free_str_array(split_point);
		return (false);
	}
	*vertex = vec3_add(*(t_vec3 *)model->vertices.array[i - 1],
			model->position);
	i = 0;
	if (split_point[1])
		i = ft_atoi(split_point[1]);
	if (i > 0)
		*uv = *(t_vec2 *)model->vertex_uv.array[i - 1];
	free_str_array(split_point);
	return (true);
}

static void	init_triangle_props(t_object *triangle, t_material *mat)
{
	triangle->type = OBJ_TRIANGLE;
	triangle->position = triangle->props.a;
	triangle->direction = vec3_normalize(vec3_cross(
				vec3_sub(triangle->props.c, triangle->props.a),
				vec3_sub(triangle->props.b, triangle->props.a)));
	triangle->props.b = vec3_sub(triangle->props.b, triangle->props.a);
	triangle->props.c = vec3_sub(triangle->props.c, triangle->props.a);
	triangle->props.a = (t_vec3){{0, 0, 0}};
	if (mat)
		triangle->color = linear_to_srgb(mat->diff_color);
	else
		triangle->color = (t_vec3){{255, 0, 255}};
}

static bool	parse_triangle_face(char **split, t_object_model *model,
		t_material *mat, int tri_index)
{
	t_object	*tri;
	bool		p_res;

	tri = ft_calloc(1, sizeof(t_object));
	if (!tri)
	{
		free_str_array(split);
		return (0);
	}
	p_res = parse_face_values(split[1], model, &tri->props.a, &tri->uv.tex_a);
	p_res &= parse_face_values(split[tri_index], model, &tri->props.b,
			&tri->uv.tex_b);
	p_res &= parse_face_values(split[tri_index + 1], model, &tri->props.c,
			&tri->uv.tex_c);
	if (p_res)
	{
		tri->material = mat;
		init_triangle_props(tri, mat);
		ft_arrayadd_back(&model->triangles, tri, free_object);
		return (true);
	}
	free(tri);
	return (false);
}

bool	parse_face(char *line, t_object_model *model,
		t_material *mat, int line_nb)
{
	char		**split;
	bool		p_res;
	int			i;

	split = ft_split_by_whitespace(line);
	if (!split)
		return (false);
	if (check_array_size(split, 4, "f", line_nb))
	{
		free_str_array(split);
		return (false);
	}
	i = 2;
	p_res = 1;
	while (p_res && split[i] && split[i + 1] && ft_strlen(split[i + 1]))
	{
		p_res = parse_triangle_face(split, model, mat, i);
		i++;
	}
	free_str_array(split);
	return (p_res);
}
