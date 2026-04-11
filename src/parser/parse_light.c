/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 00:57:05 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size)
{
	ft_putstr_fd("Error\nparser: light vector: wrong number of \
			coordinates components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)", 2);
}

static int	parse_light_vec(char *l_param, t_vec3 *v_res)
{
	char **param_split;
	int	size;

	param_split = ft_split(l_param, ',');
	if (!param_split)
	{
		ft_putstr_fd("Error\nparser: light vector: allocation failed", 2);
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
		ft_putstr_fd("Error\nparser: light vector: non-numeric value\
			detected", 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

static int	parse_light_double(char *sp_param, double *res)
{
	if (!is_numeric(sp_param))
	{
		ft_putstr_fd("Error\nparser: light value: non-numeric value\
			detected", 2);
		return (0);
	}
	*res = ft_atod(sp_param);
	return (1);
}

static int	check_array_size(char **array)
{
	int	array_size;

	array_size = 0;
	while (array[array_size])
		(array_size)++;
	if (array_size < 4)
		ft_putstr_fd("Error\nparser: light: missing parameter(s)", 2);
	return (array_size < 4);
}

int	parse_light(char **line_split, t_scene *scene)
{
	int		parse_result;
	t_light	*light;

	parse_result = 1;
	if (check_array_size(line_split))
		return (0);
	light = malloc(sizeof(t_light));
	if (!light)
	{
		ft_putstr_fd("Error\nparser: light: allocation failed", 2);
		return (0);
	}
	parse_result &= parse_light_vec(line_split[1], &(light->position));
	parse_result &= parse_light_double(line_split[2], &(light->intensity));
	parse_result &= parse_light_vec(line_split[3], &(light->color));
	ft_lstadd_back(&scene->lights, ft_lstnew(light));
	return (parse_result);
}
