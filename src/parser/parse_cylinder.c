/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 19:09:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_cylinder_elements(char **line_split, t_cylinder *cy,
	int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(cy->center), "cylinder", line_nb);
	p_res &= parse_vector(line_split[2], &(cy->axis), "cylinder", line_nb);
	p_res &= parse_double(line_split[3], &(cy->radius), "cylinder", line_nb);
	p_res &= parse_double(line_split[4], &(cy->height), "cylinder", line_nb);
	p_res &= parse_vector(line_split[5], &(cy->color), "cylinder", line_nb);
	cy->specular = 0;
	cy->shininess = 1;
	if (line_split[6] && line_split[6][0] != '\0')
		p_res &= parse_double(line_split[6], &(cy->specular), "cylinder",
				line_nb);
	if (line_split[6] && line_split[7])
		p_res &= parse_double(line_split[7], &(cy->shininess), "cylinder",
				line_nb);
	if (p_res)
	{
		cy->axis = vec3_normalize(cy->axis);
		cy->radius /= 2;
		cy->transform_axis = vec_get_matrix_rotation_z(cy->axis);
	}
	return (p_res);
}

int	parse_cylinder(char **line_split, t_scene *scene, int line_nb)
{
	int			parse_result;
	t_cylinder	*cy;

	parse_result = 1;
	if (check_array_size(line_split, 4, "cylinder", line_nb))
		return (0);
	cy = malloc(sizeof(t_cylinder));
	if (!cy)
	{
		print_parse_error("allocation failed", "cylinder", line_nb);
		return (0);
	}
	parse_result = parse_cylinder_elements(line_split, cy, line_nb);
	ft_lstadd_back(&scene->cylinder, ft_lstnew(cy));
	return (parse_result);
}
