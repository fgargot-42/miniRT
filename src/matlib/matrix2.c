/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 18:28:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/29 21:49:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matlib.h"
#include "veclib.h"
#include "libft.h"
#include <math.h>
#include <stdlib.h>

double	det2(t_mat2 mat)
{
	return (mat.mat[0][0] * mat.mat[1][1] - mat.mat[0][1] * mat.mat[1][0]);
}

t_mat2	init_mat2(t_vec2 a, t_vec2 b)
{
	t_mat2	mat;

	mat.mat[0][0] = a.x;
	mat.mat[0][1] = a.y;
	mat.mat[1][0] = b.x;
	mat.mat[1][1] = b.y;
	return (mat);
}

t_mat2	inv_mat2(t_mat2 mat)
{
	double	det;
	t_mat2	inv;

	det = det2(mat);
	if (!det)
		return (mat);
	inv.mat[0][0] = mat.mat[1][1] / det;
	inv.mat[0][1] = -mat.mat[0][1] / det;
	inv.mat[1][0] = -mat.mat[1][0] / det;
	inv.mat[1][1] = mat.mat[0][0] / det;
	return (inv);
}
