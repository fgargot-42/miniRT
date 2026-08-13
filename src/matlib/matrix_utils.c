/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 18:28:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 01:04:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "matlib.h"
#include "libft.h"
#include <math.h>
#include <stdlib.h>

t_mat3	vec_get_matrix_rotation_x(double theta)
{
	t_mat3	m;
	double	c;
	double	s;

	c = cos(theta);
	s = sin(theta);
	ft_bzero(&m, sizeof(t_mat3));
	m.mat[0][0] = 1;
	m.mat[1][1] = c;
	m.mat[1][2] = -s;
	m.mat[2][1] = s;
	m.mat[2][2] = c;
	return (m);
}

t_mat3	vec_get_matrix_rotation_y(double theta)
{
	t_mat3	m;
	double	c;
	double	s;

	c = cos(theta);
	s = sin(theta);
	ft_bzero(&m, sizeof(t_mat3));
	m.mat[0][0] = c;
	m.mat[0][2] = s;
	m.mat[1][1] = 1;
	m.mat[2][0] = -s;
	m.mat[2][2] = c;
	return (m);
}

t_mat3	vec_get_matrix_rotation_z(double theta)
{
	t_mat3	m;
	double	c;
	double	s;

	c = cos(theta);
	s = sin(theta);
	ft_bzero(&m, sizeof(t_mat3));
	m.mat[0][0] = c;
	m.mat[0][1] = -s;
	m.mat[1][0] = s;
	m.mat[1][1] = c;
	m.mat[2][2] = 1;
	return (m);
}

t_vec3	vec_apply_matrix(t_vec3 v_from, t_mat3 t_matrix)
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

t_vec3	vec_reverse_matrix(t_vec3 v_from, t_mat3 t_matrix)
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
