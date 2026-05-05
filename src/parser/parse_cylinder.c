/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 22:03:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_cylinder_elements(char **line_split, t_object *obj,
	int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(obj->position), "cylinder",
			line_nb);
	p_res &= parse_vector(line_split[2], &(obj->direction), "cylinder",
			line_nb);
	p_res &= parse_double(line_split[3], &(obj->radius), "cylinder",
			line_nb);
	p_res &= parse_double(line_split[4], &(obj->props.height), "cylinder",
			line_nb);
	p_res &= parse_vector(line_split[5], &(obj->color), "cylinder", line_nb);
	if (line_split[6] && line_split[6][0] != '\0')
		p_res &= parse_double(line_split[6], &(obj->specular), "cylinder",
				line_nb);
	if (line_split[6] && line_split[7])
		p_res &= parse_double(line_split[7], &(obj->shininess), "cylinder",
				line_nb);
	return (p_res);
}

t_object	*parse_cylinder(char **line_split, int line_nb)
{
	int			parse_result;
	t_object	*obj;

	if (check_array_size(line_split, 4, "cylinder", line_nb))
		return (0);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "cylinder", line_nb);
		return (0);
	}
	obj->shininess = 1;
	parse_result = parse_cylinder_elements(line_split, obj, line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->radius /= 2;
	obj->props.transform_axis = vec_get_matrix_rotation_z(obj->direction);
	obj->type = OBJ_CYLINDER;
	return (obj);
}
