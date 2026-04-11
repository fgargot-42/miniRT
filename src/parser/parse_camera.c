/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 00:56:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size)
{
	ft_putstr_fd("Error\nparser: camera vector: wrong number of \
			coordinates components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)", 2);
}

static int	parse_camera_vec(char *c_param, t_vec3 *v_res)
{
	char **param_split;
	int	size;

	param_split = ft_split(c_param, ',');
	if (!param_split)
	{
		ft_putstr_fd("Error\nparser: camera vector: allocation failed", 2);
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
		ft_putstr_fd("Error\nparser: camera vector: non-numeric value\
			detected", 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

static int	parse_camera_double(char *c_param, double *res)
{
	if (!is_numeric(c_param))
	{
		ft_putstr_fd("Error\nparser: camera value: non-numeric value\
			detected", 2);
		return (0);
	}
	*res = ft_atod(c_param);
	return (1);
}

static int	check_array_size(char **array)
{
	int	array_size;

	array_size = 0;
	while (array[array_size])
		(array_size)++;
	if (array_size < 4)
		ft_putstr_fd("Error\nparser: camera: missing parameter(s)", 2);
	return (array_size < 4);
}

int	parse_camera(char **line_split, t_scene *scene)
{
	int			parse_result;

	parse_result = 1;
	if (check_array_size(line_split))
		return (0);
	parse_result &= parse_camera_vec(line_split[1], &(scene->cam.position));
	parse_result &= parse_camera_vec(line_split[2], &(scene->cam.direction));
	parse_result &= parse_camera_double(line_split[3], &(scene->cam.fov));
	scene->cam.pitch = 0.0;
	scene->cam.yaw = 0.0;
	return (parse_result);
}
