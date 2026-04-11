/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 01:24:07 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size)
{
	ft_putstr_fd("Error\nparser: cone vector: wrong number of \
			coordinates components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)", 2);
}

static int	parse_cone_vec(char *co_param, t_vec3 *v_res)
{
	char **param_split;
	int	size;

	param_split = ft_split(co_param, ',');
	if (!param_split)
	{
		ft_putstr_fd("Error\nparser: cone vector: allocation failed", 2);
		return (0);
	}
	size = 0;
	while(param_split[size])
		size++;
	if (size != 3)
		print_vec_parse_error(size);
	if (!is_numeric(param_split[0]) || !is_numeric(param_split[1])
		|| !is_numeric(param_split[2]))
	{
		ft_putstr_fd("Error\nparser: cone vector: non-numeric value\
			detected", 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

static int	parse_cone_double(char *co_param, double *res)
{
	if (!is_numeric(co_param))
	{
		ft_putstr_fd("Error\nparser: cone value: non-numeric value\
			detected", 2);
		return (0);
	}
	*res = ft_atod(co_param);
	return (1);
}

static int	check_array_size(char **array)
{
	int	array_size;

	array_size = 0;
	while (array[array_size])
		(array_size)++;
	if (array_size < 6)
		ft_putstr_fd("Error\nparser: cone: missing parameter(s)", 2);
	return (array_size < 6);
}

int	parse_cone(char **line_split, t_scene *scene)
{
	int		parse_result;
	t_cone	*co;

	parse_result = 1;
	if (check_array_size(line_split))
		return (0);
	co = malloc(sizeof(t_cone));
	if (!co)
		ft_putstr_fd("Error\nparser: cone: allocation failed", 2);
	if (!co)
		return (0);
	parse_result &= parse_cone_vec(line_split[1], &(co->center));
	parse_result &= parse_cone_vec(line_split[2], &(co->axis));
	parse_result &= parse_cone_double(line_split[3], &(co->angle));
	parse_result &= parse_cone_double(line_split[4], &(co->height));
	parse_result &= parse_cone_double(line_split[5], &(co->depth));
	parse_result &= parse_cone_vec(line_split[6], &(co->color));
	co->specular = 0;
	co->shininess = 1;
	if (line_split[7] && line_split[7][0] != '\0')
		parse_result &= parse_cone_double(line_split[7], &(co->specular));
	if (line_split[7] && line_split[8])
		parse_result &= parse_cone_double(line_split[8], &(co->shininess));
	if (parse_result)
		co->transform_axis = vec_get_matrix_rotation_z(co->axis);
	ft_lstadd_back(&scene->cone, ft_lstnew(co));
	return (parse_result);
}
