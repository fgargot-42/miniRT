/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 18:28:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/29 21:51:25 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "matlib.h"
#include "libft.h"
#include <math.h>
#include <stdlib.h>

t_mat3	vec_get_matrix_rotation_z(t_vec3 v_from)
{
	double	s;
	double	kx;
	double	ky;
	t_mat3	matrix;

	s = vec3_length((t_vec3){{v_from.x, v_from.y, 0}});
	kx = v_from.y / s;
	ky = -v_from.x / s;
	matrix.mat[0][0] = v_from.z + kx * kx * (1 - v_from.z);
	matrix.mat[0][1] = kx * ky * (1 - v_from.z);
	matrix.mat[0][2] = -v_from.x;
	matrix.mat[1][0] = kx * ky * (1 - v_from.z);
	matrix.mat[1][1] = v_from.z + ky * ky * (1 - v_from.z);
	matrix.mat[1][2] = -v_from.y;
	matrix.mat[2][0] = v_from.x;
	matrix.mat[2][1] = v_from.y;
	matrix.mat[2][2] = v_from.z;
	return (matrix);
}

t_vec3	vec_apply_rotation_z(t_vec3 v_from, t_mat3 t_matrix)
{
	t_vec3	v_to;

	v_to.x = v_from.x * t_matrix.mat[0][0] + v_from.y * t_matrix.mat[0][1]
		+ v_from.z * t_matrix.mat[0][2];
	v_to.y = v_from.x * t_matrix.mat[1][0] + v_from.y * t_matrix.mat[1][1]
		+ v_from.z * t_matrix.mat[1][2];
	v_to.z = v_from.x * t_matrix.mat[2][0] + v_from.y * t_matrix.mat[2][1]
		+ v_from.z * t_matrix.mat[2][2];
	return (v_to);
}

t_vec3	vec_reverse_rotation(t_vec3 v_from, t_mat3 t_matrix)
{
	t_vec3	v_to;

	v_to.x = v_from.x * t_matrix.mat[0][0] + v_from.y * t_matrix.mat[1][0]
		+ v_from.z * t_matrix.mat[2][0];
	v_to.y = v_from.x * t_matrix.mat[0][1] + v_from.y * t_matrix.mat[1][1]
		+ v_from.z * t_matrix.mat[2][1];
	v_to.z = v_from.x * t_matrix.mat[0][2] + v_from.y * t_matrix.mat[1][2]
		+ v_from.z * t_matrix.mat[2][2];
	return (v_to);
}
