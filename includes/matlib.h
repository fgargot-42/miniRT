/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 20:03:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/29 22:15:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATLIB_H
# define MATLIB_H

# include "veclib.h"

typedef struct s_mat2
{
	double	mat[2][2];
}	t_mat2;

typedef struct s_mat3
{
	double	mat[3][3];
}	t_mat3;

// T_MAT2 FUNCTIONS
double	det2(t_mat2 mat);
t_mat2	init_mat2(t_vec2 a, t_vec2 b);
t_mat2	inv_mat2(t_mat2 mat);


// T_MAT3 FUNCTIONS
double	det3(t_mat3 mat);
t_mat3	init_mat3(t_vec3 a, t_vec3 b, t_vec3 c);
t_mat3	inv_mat3(t_mat3 mat);
t_mat3	transpose_mat3(t_mat3 mat);
t_vec3	mat3_vec_mult(t_mat3 mat, t_vec3 vec);

t_mat3	vec_get_matrix_rotation_z(t_vec3 v_from);
t_vec3	vec_apply_rotation_z(t_vec3 v_from, t_mat3 t_matrix);
t_vec3	vec_reverse_rotation(t_vec3 v_from, t_mat3 t_matrix);

#endif // VECLIB_H
