/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_paraboloid.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/04 22:18:49 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_paraboloid_elements(char **line_split, t_paraboloid *pa, int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(pa->center), "paraboloid", line_nb);
	p_res &= parse_vector(line_split[2], &(pa->axis), "paraboloid", line_nb);
	p_res &= parse_double(line_split[3], &(pa->angle), "paraboloid", line_nb);
	p_res &= parse_double(line_split[4], &(pa->height), "paraboloid", line_nb);
	p_res &= parse_vector(line_split[6], &(pa->color), "paraboloid", line_nb);
	pa->specular = 0;
	pa->shininess = 1;
	if (line_split[7] && ft_strlen(line_split[7]))
		p_res &= parse_double(line_split[7], &(pa->specular), "paraboloid",
				line_nb);
	if (line_split[7] && line_split[8] && ft_strlen(line_split[8]))
		p_res &= parse_double(line_split[8], &(pa->shininess), "paraboloid",
				line_nb);
	if (p_res)
	{
		pa->axis = vec3_normalize(pa->axis);
		pa->tan_angle = tan(pa->angle * M_PI / 180);
		pa->transform_axis = vec_get_matrix_rotation_z(pa->axis);
	}
	return (p_res);
}

t_object	*parse_paraboloid(char **line_split, int line_nb)
{
	int				parse_result;
	t_paraboloid	*pa;
	t_object		*obj;

	obj = NULL;
	if (check_array_size(line_split, 6, "paraboloid", line_nb))
		return (NULL);
	pa = malloc(sizeof(t_paraboloid));
	if (!pa)
		print_parse_error("allocation failed", "paraboloid", line_nb);
	if (!pa)
		return (NULL);
	parse_result = parse_paraboloid_elements(line_split, pa, line_nb);
	if (parse_result)
		obj = create_object(pa, OBJ_PARABOLOID);
	if (!obj)
		free(pa);
	return (obj);
}
