/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 18:28:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/07 21:54:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "math.h"
#include "libft.h"
#include <stdlib.h>

void	free_matrix(double **matrix)
{
	if (!matrix)
		return ;
	if (matrix[0])
		free(matrix[0]);
	if (matrix[1])
		free(matrix[1]);
	if (matrix[2])
		free(matrix[2]);
	free(matrix);
}

static double	**get_empty_matrix(void)
{
	double	**matrix;

	matrix = ft_calloc(sizeof(double *), 3);
	if (!matrix)
		return (NULL);
	matrix[0] = ft_calloc(sizeof(double), 3);
	matrix[1] = ft_calloc(sizeof(double), 3);
	matrix[2] = ft_calloc(sizeof(double), 3);
	if (!matrix[0] || !matrix[1] || !matrix[2])
	{
		free_matrix(matrix);
		return (NULL);
	}
	return (matrix);
}

double	**vec_get_matrix_rotation_z(t_vec3 v_from)
{
	double	c;
	double	s;
	double	kx;
	double	ky;
	double	**matrix;

	matrix = get_empty_matrix();
	if (!matrix)
		return (NULL);
	c = v_from.z;
	s = vec_length((t_vec3){v_from.x, v_from.y, 0});
	kx = v_from.y / s;
	ky = -v_from.x / s;
	matrix[0][0] = c + kx * kx * (1 - c);
	matrix[0][1] = kx * ky * (1 - c);
	matrix[0][2] = -ky * s;
	matrix[1][0] = kx * ky * (1 - c);
	matrix[1][1] = c + ky * ky * (1 - c);
	matrix[1][2] = kx * s;
	matrix[2][0] = ky * s;
	matrix[2][1] = -kx * s;
	matrix[2][2] = c;
	return (matrix);
}

t_vec3	vec_apply_rotation_z(t_vec3 v_from, double **t_matrix)
{
	t_vec3	v_to;

	v_to.x = v_from.x * t_matrix[0][0] + v_from.y * t_matrix[0][1]
		+ v_from.z * t_matrix[0][2];
	v_to.y = v_from.x * t_matrix[1][0] + v_from.y * t_matrix[1][1]
		+ v_from.z * t_matrix[1][2];
	v_to.z = v_from.x * t_matrix[2][0] + v_from.y * t_matrix[2][1]
		+ v_from.z * t_matrix[2][2];
	return (v_to);
}

t_vec3	vec_reverse_rotation(t_vec3 v_from, double **t_matrix)
{
	t_vec3	v_to;

	v_to.x = v_from.x * t_matrix[0][0] + v_from.y * t_matrix[1][0]
		+ v_from.z * t_matrix[2][0];
	v_to.y = v_from.x * t_matrix[0][1] + v_from.y * t_matrix[1][1]
		+ v_from.z * t_matrix[2][1];
	v_to.z = v_from.x * t_matrix[0][2] + v_from.y * t_matrix[1][2]
		+ v_from.z * t_matrix[2][2];
	return (v_to);
}
