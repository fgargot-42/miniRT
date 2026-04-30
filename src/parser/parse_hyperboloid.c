/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hyperboloid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/30 22:19:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_hyperboloid_elements(char **line_split, t_hyperboloid *hy, int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(hy->center), "hyperboloid", line_nb);
	p_res &= parse_vector(line_split[2], &(hy->axis), "hyperboloid", line_nb);
	p_res &= parse_double(line_split[3], &(hy->angle), "hyperboloid", line_nb);
	p_res &= parse_double(line_split[4], &(hy->height), "hyperboloid", line_nb);
	p_res &= parse_double(line_split[5], &(hy->depth), "hyperboloid", line_nb);
	p_res &= parse_vector(line_split[6], &(hy->color), "hyperboloid", line_nb);
	hy->specular = 0;
	hy->shininess = 1;
	if (line_split[7] && ft_strlen(line_split[7]))
		p_res &= parse_double(line_split[7], &(hy->specular), "hyperboloid",
				line_nb);
	if (line_split[7] && line_split[8] && ft_strlen(line_split[8]))
		p_res &= parse_double(line_split[8], &(hy->shininess), "hyperboloid",
				line_nb);
	if (p_res)
	{
		hy->axis = vec3_normalize(hy->axis);
		hy->tan_angle = tan(hy->angle * M_PI / 180);
		hy->transform_axis = vec_get_matrix_rotation_z(hy->axis);
	}
	return (p_res);
}

t_object	*parse_hyperboloid(char **line_split, int line_nb)
{
	int				parse_result;
	t_hyperboloid	*hy;
	t_object		*obj;

	obj = NULL;
	if (check_array_size(line_split, 6, "hyperboloid", line_nb))
		return (NULL);
	hy = malloc(sizeof(t_hyperboloid));
	if (!hy)
		print_parse_error("allocation failed", "hyperboloid", line_nb);
	if (!hy)
		return (NULL);
	parse_result = parse_hyperboloid_elements(line_split, hy, line_nb);
	if (parse_result)
		obj = create_object(hy, OBJ_HYPERBOLOID);
	if (!obj)
		free(hy);
	return (obj);
}
