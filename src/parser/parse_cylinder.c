/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 01:30:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size)
{
	ft_putstr_fd("Error\nparser: cylinder vector: wrong number of components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)\n", 2);
}

static int	parse_cylinder_vec(char *cy_param, t_vec3 *v_res)
{
	char **param_split;
	int	size;

	param_split = ft_split(cy_param, ',');
	if (!param_split)
	{
		ft_putstr_fd("Error\nparser: cylinder vector: allocation failed\n", 2);
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
		ft_putstr_fd("Error\nparser: cylinder vector: non-numeric value detected\n", 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

static int	parse_cylinder_double(char *cy_param, double *res)
{
	if (!is_numeric(cy_param))
	{
		ft_putstr_fd("Error\nparser: cylinder value: non-numeric value detected\n", 2);
		return (0);
	}
	*res = ft_atod(cy_param);
	return (1);
}

static int	check_array_size(char **array)
{
	int	array_size;

	array_size = 0;
	while (array[array_size])
		(array_size)++;
	if (array_size < 4)
		ft_putstr_fd("Error\nparser: cylinder: missing parameter(s)", 2);
	return (array_size < 4);
}

int	parse_cylinder(char **line_split, t_scene *scene)
{
	int			parse_result;
	t_cylinder	*cy;

	parse_result = 1;
	if (check_array_size(line_split))
		return (0);
	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		ft_putstr_fd("Error\nparser: cylinder: allocation failed", 2);
	if (!cy)
		return (0);
	parse_result &= parse_cylinder_vec(line_split[1], &(cy->center));
	parse_result &= parse_cylinder_vec(line_split[2], &(cy->axis));
	parse_result &= parse_cylinder_double(line_split[3], &(cy->radius));
	parse_result &= parse_cylinder_double(line_split[4], &(cy->height));
	parse_result &= parse_cylinder_vec(line_split[5], &(cy->color));
	cy->specular = 0;
	cy->shininess = 1;
	if (line_split[6] && line_split[6][0] != '\0')
		parse_result &= parse_cylinder_double(line_split[6], &(cy->specular));
	if (line_split[6] && line_split[7])
		parse_result &= parse_cylinder_double(line_split[7], &(cy->shininess));
	if (parse_result)
	{
		cy->radius /= 2;
		cy->transform_axis = vec_get_matrix_rotation_z(cy->axis);
	}
	ft_lstadd_back(&scene->cylinder, ft_lstnew(cy));
	return (parse_result);
}
