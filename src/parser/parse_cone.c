/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 20:06:54 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_cone_elements(char **line_split, t_cone *co, int line_nb)
{
	int	parse_result;

	parse_result = parse_vector(line_split[1], &(co->center), "cone", line_nb);
	parse_result &= parse_vector(line_split[2], &(co->axis), "cone", line_nb);
	parse_result &= parse_double(line_split[3], &(co->angle), "cone", line_nb);
	parse_result &= parse_double(line_split[4], &(co->height), "cone", line_nb);
	parse_result &= parse_double(line_split[5], &(co->depth), "cone", line_nb);
	parse_result &= parse_vector(line_split[6], &(co->color), "cone", line_nb);
	co->specular = 0;
	co->shininess = 1;
	if (line_split[7] && ft_strlen(line_split[7]))
		parse_result &= parse_double(line_split[7], &(co->specular), "cone",
				line_nb);
	if (line_split[7] && line_split[8] && ft_strlen(line_split[8]))
		parse_result &= parse_double(line_split[8], &(co->shininess), "cone",
				line_nb);
	if (parse_result)
		co->transform_axis = vec_get_matrix_rotation_z(co->axis);
	return (parse_result);
}

int	parse_cone(char **line_split, t_scene *scene, int line_nb)
{
	int		parse_result;
	t_cone	*co;

	if (check_array_size(line_split, 6, "cone", line_nb))
		return (0);
	co = malloc(sizeof(t_cone));
	if (!co)
		print_parse_error("allocation failed", "cone", line_nb);
	if (!co)
		return (0);
	parse_result = parse_cone_elements(line_split, co, line_nb);
	ft_lstadd_back(&scene->cone, ft_lstnew(co));
	return (parse_result);
}
