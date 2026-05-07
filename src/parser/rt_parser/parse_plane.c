/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 22:04:15 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_plane_elements(char **line_split, t_object *obj, int line_nb)
{
	int	parse_result;

	parse_result = parse_vector(line_split[1], &(obj->position), "plane",
			line_nb);
	parse_result &= parse_vector(line_split[2], &(obj->direction), "plane",
			line_nb);
	parse_result &= parse_vector(line_split[3], &(obj->color), "plane",
			line_nb);
	obj->checker = ft_atoi(line_split[4]);
	if (line_split[5] && ft_strlen(line_split[5]))
		parse_result &= parse_double(line_split[5], &(obj->specular),
				"plane", line_nb);
	if (line_split[5] && line_split[6] && ft_strlen(line_split[6]))
		parse_result &= parse_double(line_split[6], &(obj->shininess),
				"plane", line_nb);
	return (parse_result);
}

t_object	*parse_plane(char **line_split, int line_nb)
{
	int			parse_result;
	t_object	*obj;

	obj = NULL;
	if (check_array_size(line_split, 5, "plane", line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "plane", line_nb);
		return (NULL);
	}
	obj->shininess = 1;
	parse_result = parse_plane_elements(line_split, obj, line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->direction = vec3_normalize(obj->direction);
	obj->type = OBJ_PLANE;
	return (obj);
}
