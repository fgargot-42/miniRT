/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 18:28:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/18 00:27:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matlib.h"
#include "veclib.h"
#include "libft.h"
#include <math.h>
#include <stdlib.h>

double	det3(t_mat3 mat)
{
	double	det;

	det = mat.mat[0][0] * mat.mat[1][1] * mat.mat[2][2];
	det += mat.mat[0][1] * mat.mat[1][2] * mat.mat[2][0];
	det += mat.mat[0][2] * mat.mat[1][0] * mat.mat[2][1];
	det -= mat.mat[0][2] * mat.mat[1][1] * mat.mat[2][0];
	det -= mat.mat[0][1] * mat.mat[1][0] * mat.mat[2][2];
	det -= mat.mat[0][0] * mat.mat[1][2] * mat.mat[2][1];
	return (det);
}

t_mat3	init_mat3(t_vec3 a, t_vec3 b, t_vec3 c)
{
	t_mat3	mat;

	mat.mat[0][0] = a.x;
	mat.mat[0][1] = a.y;
	mat.mat[0][2] = a.z;
	mat.mat[1][0] = b.x;
	mat.mat[1][1] = b.y;
	mat.mat[1][2] = b.z;
	mat.mat[2][0] = c.x;
	mat.mat[2][1] = c.y;
	mat.mat[2][2] = c.z;
	return (mat);
}

t_mat3	inv_mat3(t_mat3 mat)
{
	double	det;
	t_mat3	inv;

	det = det3(mat);
	if (!det)
		return (mat);
	inv.mat[0][0] = (mat.mat[1][1] * mat.mat[2][2]
			- mat.mat[2][1] * mat.mat[1][2]) / det;
	inv.mat[0][1] = (mat.mat[0][2] * mat.mat[2][1]
			- mat.mat[0][1] * mat.mat[2][2]) / det;
	inv.mat[0][2] = (mat.mat[0][1] * mat.mat[1][2]
			- mat.mat[2][0] * mat.mat[1][1]) / det;
	inv.mat[1][0] = (mat.mat[1][2] * mat.mat[2][0]
			- mat.mat[1][0] * mat.mat[2][2]) / det;
	inv.mat[1][1] = (mat.mat[0][0] * mat.mat[2][2]
			- mat.mat[0][2] * mat.mat[2][0]) / det;
	inv.mat[1][2] = (mat.mat[0][2] * mat.mat[1][0]
			- mat.mat[0][0] * mat.mat[1][2]) / det;
	inv.mat[2][0] = (mat.mat[1][0] * mat.mat[2][1]
			- mat.mat[1][1] * mat.mat[2][0]) / det;
	inv.mat[2][1] = (mat.mat[0][1] * mat.mat[2][0]
			- mat.mat[0][0] * mat.mat[2][1]) / det;
	inv.mat[2][2] = (mat.mat[0][0] * mat.mat[1][1]
			- mat.mat[0][1] * mat.mat[1][0]) / det;
	return (inv);
}

t_mat3	transpose_mat3(t_mat3 mat)
{
	t_mat3	tr;

	tr.mat[0][0] = mat.mat[0][0];
	tr.mat[0][1] = mat.mat[1][0];
	tr.mat[0][2] = mat.mat[2][0];
	tr.mat[1][0] = mat.mat[0][1];
	tr.mat[1][1] = mat.mat[1][1];
	tr.mat[1][2] = mat.mat[2][1];
	tr.mat[2][0] = mat.mat[0][2];
	tr.mat[2][1] = mat.mat[1][2];
	tr.mat[2][2] = mat.mat[2][2];
	return (tr);
}

t_vec3	mat3_vec_mult(t_mat3 mat, t_vec3 vec)
{
	t_vec3	res;

	res = vec3_scale((t_vec3){{mat.mat[0][0], mat.mat[0][1],
			mat.mat[0][2]}}, vec.x);
	res = vec3_add(res, vec3_scale((t_vec3){{.x = mat.mat[1][0],
				.y = mat.mat[1][1], .z = mat.mat[1][2]}}, vec.y));
	res = vec3_add(res, vec3_scale((t_vec3){{.x = mat.mat[2][0],
				.y = mat.mat[2][1], .z = mat.mat[2][2]}}, vec.z));
	return (res);
}
