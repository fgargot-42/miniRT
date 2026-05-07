/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_paraboloid.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 22:03:53 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_paraboloid_elements(char **line_split, t_object *obj,
		int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(obj->position), "paraboloid",
			line_nb);
	p_res &= parse_vector(line_split[2], &(obj->direction), "paraboloid",
			line_nb);
	p_res &= parse_double(line_split[3], &(obj->angle), "paraboloid", line_nb);
	p_res &= parse_double(line_split[4], &(obj->props.height), "paraboloid",
			line_nb);
	p_res &= parse_vector(line_split[6], &(obj->color), "paraboloid", line_nb);
	if (line_split[7] && ft_strlen(line_split[7]))
		p_res &= parse_double(line_split[7], &(obj->specular), "paraboloid",
				line_nb);
	if (line_split[7] && line_split[8] && ft_strlen(line_split[8]))
		p_res &= parse_double(line_split[8], &(obj->shininess), "paraboloid",
				line_nb);
	return (p_res);
}

t_object	*parse_paraboloid(char **line_split, int line_nb)
{
	int				parse_result;
	t_object		*obj;

	if (check_array_size(line_split, 6, "paraboloid", line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "paraboloid", line_nb);
		return (NULL);
	}
	obj->shininess = 1;
	parse_result = parse_paraboloid_elements(line_split, obj, line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->props.tan_angle = tan(obj->angle * M_PI / 180);
	obj->props.transform_axis = vec_get_matrix_rotation_z(obj->direction);
	obj->type = OBJ_PARABOLOID;
	return (obj);
}
