/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:39:33 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 20:24:02 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	print_vec_parse_error(int size, char *object, int line_nb)
{
	ft_putstr_fd("Error\nLine ", 2);
	ft_putnbr_fd(line_nb, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(object, 2);
	ft_putstr_fd(": wrong number of coordinates components (got ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(", expected 3)\n", 2);
}

int	check_array_size(char **array, int expected, char *object, int line_nb)
{
	int	array_size;
	int	i;

	array_size = 0;
	while (array && array[array_size])
		(array_size)++;
	if (array_size < expected)
	{
		i = 0;
		print_parse_error("missing parameter(s)", object, line_nb);
		while (i < array_size)
		{
			ft_putstr_fd("\t", 2);
			ft_putstr_fd(array[i], 2);
			i++;
		}
		ft_putstr_fd("\n", 2);
	}
	return (array_size < expected);
}

static char	**parse_vector_split(char *param, char *object, int line_nb)
{
	char	**param_split;
	int		size;

	size = 0;
	param_split = ft_split(param, ',');
	if (!param_split)
	{
		print_parse_error("vector: allocation failed", object, line_nb);
		return (NULL);
	}
	while (param_split[size])
		size++;
	if (size != 3)
	{
		print_vec_parse_error(size, object, line_nb);
		size = 0;
		while (param_split[size])
		{
			free(param_split[size]);
			size++;
		}
		free(param_split);
		return (NULL);
	}
	return (param_split);
}

int	parse_vector(char *param, t_vec3 *v_res, char *object, int line_nb)
{
	char	**param_split;

	param_split = parse_vector_split(param, object, line_nb);
	if (!param_split)
		return (0);
	if (!is_numeric(param_split[0]) || !is_numeric(param_split[1])
		|| !is_numeric(param_split[2]))
	{
		print_parse_error("vector: non-numeric value detected", object,
			line_nb);
		ft_putendl_fd(param, 2);
		return (0);
	}
	v_res->x = ft_atod(param_split[0]);
	v_res->y = ft_atod(param_split[1]);
	v_res->z = ft_atod(param_split[2]);
	return (1);
}

int	parse_double(char *param, double *res, char *object, int line_nb)
{
	if (!is_numeric(param))
	{
		print_parse_error("number: non-numeric value detected", object,
			line_nb);
		ft_putendl_fd(param, 2);
		return (0);
	}
	*res = ft_atod(param);
	return (1);
}
