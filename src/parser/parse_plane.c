/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 01:08:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size)
{
	ft_putstr_fd("Error\nparser: plane vector: wrong number of \
			coordinates components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)", 2);
}

static int	parse_plane_vec(char *pl_param, t_vec3 *v_res)
{
	char **param_split;
	int	size;

	param_split = ft_split(pl_param, ',');
	if (!param_split)
	{
		ft_putstr_fd("Error\nparser: plane vector: allocation failed", 2);
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
		ft_putstr_fd("Error\nparser: plane vector: non-numeric value\
			detected", 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

static int	parse_plane_double(char *pl_param, double *res)
{
	if (!is_numeric(pl_param))
	{
		ft_putstr_fd("Error\nparser: plane value: non-numeric value\
			detected", 2);
		return (0);
	}
	*res = ft_atod(pl_param);
	return (1);
}

static int	check_array_size(char **array)
{
	int	array_size;

	array_size = 0;
	while (array[array_size])
		(array_size)++;
	if (array_size < 5)
		ft_putstr_fd("Error\nparser: plane: missing parameter(s)", 2);
	return (array_size < 5);
}

int	parse_plane(char **line_split, t_scene *scene)
{
	int		parse_result;
	t_plane	*pl;

	parse_result = 1;
	if (check_array_size(line_split))
		return (0);
	pl = malloc(sizeof(t_plane));
	if (!pl)
		ft_putstr_fd("Error\nparser: plane: allocation failed", 2);
	if (!pl)
		return (0);
	parse_result &= parse_plane_vec(line_split[1], &(pl->point));
	parse_result &= parse_plane_vec(line_split[2], &(pl->normal));
	parse_result &= parse_plane_vec(line_split[3], &(pl->color));
	pl->checker = ft_atoi(line_split[4]);
	pl->specular = 0;
	pl->shininess = 1;
	if (line_split[5] && line_split[5][0] != '\0')
		parse_result &= parse_plane_double(line_split[5], &(pl->specular));
	if (line_split[5] && line_split[6])
		parse_result &= parse_plane_double(line_split[6], &(pl->shininess));
	ft_lstadd_back(&scene->planes, ft_lstnew(pl));
	return (parse_result);
}
