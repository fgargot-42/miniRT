/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 01:30:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size)
{
	ft_putstr_fd("Error\nparser: sphere vector: wrong number of \
			coordinates components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)", 2);
}

static int	parse_sphere_vec(char *sp_param, t_vec3 *v_res)
{
	char **param_split;
	int	size;

	param_split = ft_split(sp_param, ',');
	if (!param_split)
	{
		ft_putstr_fd("Error\nparser: sphere vector: allocation failed", 2);
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
		ft_putstr_fd("Error\nparser: sphere vector: non-numeric value\
			detected", 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

static int	parse_sphere_double(char *sp_param, double *res)
{
	if (!is_numeric(sp_param))
	{
		ft_putstr_fd("Error\nparser: sphere value: non-numeric value\
			detected", 2);
		return (0);
	}
	*res = ft_atod(sp_param);
	return (1);
}

static int	check_array_size(char **array)
{
	int array_size;

	array_size = 0;
	while (array[array_size])
		(array_size)++;
	if (array_size < 4)
		ft_putstr_fd("Error\nparser: sphere: missing parameter(s)", 2);
	return (array_size < 4);
}

int	parse_sphere(char **line_split, t_scene *scene)
{
	int			parse_result;
	t_sphere	*sp;

	parse_result = 1;
	if (check_array_size(line_split))
		return (0);
	sp = malloc(sizeof(t_sphere));
	if (!sp)
		ft_putstr_fd("Error\nparser: sphere: allocation failed", 2);
	if (!sp)
		return (0);
	parse_result &= parse_sphere_vec(line_split[1], &(sp->center));
	parse_result &= parse_sphere_double(line_split[2], &(sp->radius));
	parse_result &= parse_sphere_vec(line_split[3], &(sp->color));
	sp->specular = 0;
	sp->shininess = 1;
	if (line_split[4])
		parse_result &= parse_sphere_double(line_split[4], &(sp->specular));
	if (line_split[4] && line_split[5])
		parse_result &= parse_sphere_double(line_split[5], &(sp->shininess));
	if (parse_result)
		sp->radius /= 2;
	ft_lstadd_back(&scene->spheres, ft_lstnew(sp));
	return (parse_result);
}
