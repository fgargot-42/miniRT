/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj_face.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 23:09:20 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/08 02:05:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "object.h"
#include "miniRT.h"

static void print_vertex(void *c)
{
	t_vec3	*v;

	v = (t_vec3 *)c;
	printf("Vertex: x=%.2f, y=%.2f, z=%.2f\n", v->x, v->y, v->z);
}

static int	parse_face_a_values(char *line_split, t_object_model *model,
	t_object *triangle)
{
	char 	**split_point;
	double	i;
	t_list	*lst;
	
	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (0);
	i = ft_atoi(split_point[0]);
	printf("%f\n", i);
	ft_lstiter(model->vertex_list, print_vertex);
	printf("------------------------\n");
	lst = ft_lstget_elem_index(model->vertex_list, i - 1);
	if (lst)
		triangle->props.a = *((t_vec3 *)(lst->content));
	free_str_array(split_point);
	return (1);
}

static int	parse_face_b_values(char *line_split, t_object_model *model,
	t_object *triangle, int line_nb)
{
	char 	**split_point;
	double	i;
	int		status;
	t_list	*lst;
	
	i = -1;
	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (0);
	status = parse_double(split_point[0], &i, "face vertex", line_nb);
	lst = ft_lstget_elem_index(model->vertex_list, i - 1);
	if (lst)
		triangle->props.b = *((t_vec3 *)(lst->content));
	free_str_array(split_point);
	return (status);
}

static int	parse_face_c_values(char *line_split, t_object_model *model,
	t_object *triangle, int line_nb)
{
	char 	**split_point;
	double	i;
	int		status;
	t_list	*lst;
	
	i = -1;
	split_point = ft_split_keep_empty(line_split, '/');
	if (!split_point)
		return (0);
	status = parse_double(split_point[0], &i, "face vertex", line_nb);
	lst = ft_lstget_elem_index(model->vertex_list, i - 1);
	if (lst)
		triangle->props.c = *((t_vec3 *)(lst->content));
	free_str_array(split_point);
	return (status);
}

int	parse_face(char *line, t_object_model *model, t_material *mat, int line_nb)
{
	char		**split;
	t_object	*triangle;
	int			parse_result;

	split = ft_split_by_whitespace(line);
	//ft_lstiter(model->vertex_list, print_vertex);
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
	triangle->type = OBJ_TRIANGLE;
	parse_result = parse_face_a_values(split[1], model, triangle);
	parse_result &= parse_face_b_values(split[2], model, triangle, line_nb);
	parse_result &= parse_face_c_values(split[3], model, triangle, line_nb);
	if (mat)
	{
		triangle->color = vec3_scale(mat->diffuse, 255); // not exact scaling, uses a more complex and non-linear scaling
	}	
	if (parse_result)
		ft_lstadd_back(&(model->triangles), ft_lstnew(triangle));
	free_str_array(split);
	return (parse_result);
}
