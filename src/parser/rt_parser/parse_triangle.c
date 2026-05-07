/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 22:53:49 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/06 23:55:08 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_triangle_elements(char **line_split, t_object *obj,
	int line_nb)
{
	int	parse_result;

	parse_result = parse_vector(line_split[1], &(obj->props.a), "triangle",
			line_nb);
	parse_result &= parse_vector(line_split[2], &(obj->props.b), "triangle",
			line_nb);
	parse_result &= parse_vector(line_split[3], &(obj->props.c), "triangle",
			line_nb);
	parse_result &= parse_vector(line_split[4], &(obj->color), "triangle",
			line_nb);
	obj->checker = ft_atoi(line_split[5]);
	if (line_split[6] && ft_strlen(line_split[6]))
		parse_result &= parse_double(line_split[6], &(obj->specular),
				"triangle", line_nb);
	if (line_split[6] && line_split[7] && ft_strlen(line_split[7]))
		parse_result &= parse_double(line_split[7], &(obj->shininess),
				"triangle", line_nb);
	return (parse_result);
}

t_object	*parse_triangle(char **line_split, int line_nb)
{
	int			parse_result;
	t_object	*obj;

	obj = NULL;
	if (check_array_size(line_split, 6, "triangle", line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "triangle", line_nb);
		return (NULL);
	}
	obj->shininess = 1;
	parse_result = parse_triangle_elements(line_split, obj, line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->position = obj->props.a;
	obj->direction = vec3_normalize(vec3_cross(vec3_sub(obj->props.c, obj->props.a),
		vec3_sub(obj->props.b, obj->props.a)));
	obj->type = OBJ_TRIANGLE;
	return (obj);
}
