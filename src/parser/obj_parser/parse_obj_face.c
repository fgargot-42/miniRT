/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj_face.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 23:09:20 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/26 19:48:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "object.h"
#include "miniRT.h"
#include <assert.h>

static int	parse_face_a_values(char *line_split, t_object_model *model,
	t_object *triangle)
{
	char	**split_point;
	int		i;
	t_list	*lst;

	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (0);
	i = ft_atoi(split_point[0]);
	lst = ft_lstget_elem_index(model->vertex_list, i - 1);
	if (lst)
		triangle->props.a = vec3_add(*((t_vec3 *)(lst->content)), model->position);
	i = 0;
	if (split_point[1])
	{
		i = ft_atoi(split_point[1]);
		lst = ft_lstget_elem_index(model->texture_uv_list, i - 1);
		if (lst)
			triangle->texture.tex_a = *((t_vec2 *)(lst->content));
	}
	free_str_array(split_point);
	return (1);
}

static int	parse_face_b_values(char *line_split, t_object_model *model,
	t_object *triangle)
{
	char	**split_point;
	int		i;
	t_list	*lst;

	i = -1;
	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (0);
	i = ft_atoi(split_point[0]);
	lst = ft_lstget_elem_index(model->vertex_list, i - 1);
	if (lst)
		triangle->props.b = vec3_add(*((t_vec3 *)(lst->content)), model->position);
	i = 0;
	if (split_point[1])
	{
		i = ft_atoi(split_point[1]);
		lst = ft_lstget_elem_index(model->texture_uv_list, i - 1);
		if (lst)
			triangle->texture.tex_b = *((t_vec2 *)(lst->content));
	}
	free_str_array(split_point);
	return (1);
}

static int	parse_face_c_values(char *line_split, t_object_model *model,
	t_object *triangle)
{
	char	**split_point;
	int		i;
	t_list	*lst;

	i = -1;
	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (0);
	i = ft_atoi(split_point[0]);
	lst = ft_lstget_elem_index(model->vertex_list, i - 1);
	if (lst)
		triangle->props.c = vec3_add(*((t_vec3 *)(lst->content)), model->position);
	i = 0;
	if (split_point[1])
	{
		i = ft_atoi(split_point[1]);
		lst = ft_lstget_elem_index(model->texture_uv_list, i - 1);
		if (lst)
			triangle->texture.tex_c = *((t_vec2 *)(lst->content));
	}
	free_str_array(split_point);
	return (1);
}

static void	init_triangle_props(t_object *triangle, t_material *mat)
{
	triangle->type = OBJ_TRIANGLE;
	triangle->shininess = 1;
	triangle->direction = vec3_normalize(vec3_cross(
				vec3_sub(triangle->props.c, triangle->props.a),
				vec3_sub(triangle->props.b, triangle->props.a)));
	if (mat)
		triangle->color = linear_to_srgb(mat->diffuse);
	else
		triangle->color = (t_vec3){255, 0, 255};
}

int	parse_face(char *line, t_object_model *model, t_material *mat, int line_nb)
{
	char		**split;
	t_object	*triangle;
	int			parse_result;

	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	triangle = ft_calloc(1, sizeof(t_object));
	if (!triangle || check_array_size(split, 4, "f", line_nb))
	{
		if (triangle)
			free(triangle);
		free_str_array(split);
		return (0);
	}
	parse_result = parse_face_a_values(split[1], model, triangle);
	parse_result &= parse_face_b_values(split[2], model, triangle);
	parse_result &= parse_face_c_values(split[3], model, triangle);
	init_triangle_props(triangle, mat);
	triangle->tex = model->tex;
	if (parse_result)
		ft_lstadd_back(&(model->triangles), ft_lstnew(triangle));
	free_str_array(split);
	return (parse_result);
}
