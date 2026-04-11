/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 00:56:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size)
{
	ft_putstr_fd("Error\nparser: ambient vector: wrong number of \
			coordinates components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)", 2);
}

static int	parse_ambient_vec(char *a_param, t_vec3 *v_res)
{
	char	**param_split;
	int		size;

	param_split = ft_split(a_param, ',');
	if (!param_split)
	{
		ft_putstr_fd("Error\nparser: ambient vector: allocation failed", 2);
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
		ft_putstr_fd("Error\nparser: ambient vector: non-numeric value\
			detected", 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

static int	parse_ambient_double(char *a_param, double *res)
{
	if (!is_numeric(a_param))
	{
		ft_putstr_fd("Error\nparser: ambient value: non-numeric value\
			detected", 2);
		return (0);
	}
	*res = ft_atod(a_param);
	return (1);
}

static int	check_array_size(char **array)
{
	int	array_size;

	array_size = 0;
	while (array[array_size])
		(array_size)++;
	if (array_size < 3)
		ft_putstr_fd("Error\nparser: ambient: missing parameter(s)", 2);
	return (array_size < 3);
}

int	parse_ambient(char **line_split, t_scene *scene)
{
	int		parse_result;
	double	intensity;

	parse_result = 1;
	if (check_array_size(line_split))
		return (0);
	parse_result &= parse_ambient_vec(line_split[2], &(scene->ambient));
	parse_result &= parse_ambient_double(line_split[1], &intensity);
	if (parse_result)
		scene->ambient = vec_scale(scene->ambient, intensity);
	return (parse_result);
}
