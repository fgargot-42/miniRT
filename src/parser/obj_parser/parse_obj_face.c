/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj_face.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 23:09:20 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/18 23:56:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "object.h"
#include "miniRT.h"
#include <assert.h>

static int	parse_face_values(char *line_split, t_object_model *model,
	t_vec3 *vertex, t_vec2* uv)
{
	char	**split_point;
	int		i;

	if (!model || !vertex || !uv)
		return (0);
	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (0);
	i = ft_atoi(split_point[0]);
	if (i < 1)
	{
		free_str_array(split_point);
		return (0);
	}
	*vertex = vec3_add(*(t_vec3 *)model->vertices.array[i - 1], model->position);
	i = 0;
	if (split_point[1])
		i = ft_atoi(split_point[1]);
	if (i > 0)
		*uv = *(t_vec2 *)model->vertex_uv.array[i - 1];
	free_str_array(split_point);
	return (1);
}

static void	init_triangle_props(t_object *triangle, t_material *mat)
{
	triangle->type = OBJ_TRIANGLE;
	triangle->shininess = 1;
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

int	parse_face(char *line, t_object_model *model, t_material *mat, int line_nb)
{
	char		**split;
	t_object	*tri;
	int			p_res;

	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	tri = ft_calloc(1, sizeof(t_object));
	if (!tri || check_array_size(split, 4, "f", line_nb))
	{
		if (tri)
			free(tri);
		free_str_array(split);
		return (0);
	}
	p_res = parse_face_values(split[1], model, &tri->props.a, &tri->uv.tex_a);
	p_res &= parse_face_values(split[2], model, &tri->props.b, &tri->uv.tex_b);
	p_res &= parse_face_values(split[3], model, &tri->props.c, &tri->uv.tex_c);
	init_triangle_props(tri, mat);
	tri->material = mat;
	if (p_res)
		ft_arrayadd_back(&model->triangles, tri, free_object);
	free_str_array(split);
	return (p_res);
}
