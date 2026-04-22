/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   veclib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 20:03:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 20:04:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECLIB_H
# define VECLIB_H

typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

// T_VEC3 FUNCTIONS

t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_scale(t_vec3 v, double scalar);
double	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
double	vec3_length(t_vec3 v);
t_vec3	vec3_normalize(t_vec3 v);
t_vec3	vec3_multiply(t_vec3 a, t_vec3 b);
t_vec3	vec3_clamp(t_vec3 v, double min, double max);
double	vec3_distance(t_vec3 a, t_vec3 b);

void	free_matrix(double **matrix);
double	**vec_get_matrix_rotation_z(t_vec3 v_from);
t_vec3	vec_apply_rotation_z(t_vec3 v_from, double **t_matrix);
t_vec3	vec_reverse_rotation(t_vec3 v_from, double **t_matrix);

// T_VEC2 FUNCTIONS

t_vec2	vec2_add(t_vec2 a, t_vec2 b);
t_vec2	vec2_sub(t_vec2 a, t_vec2 b);
t_vec2	vec2_scale(t_vec2 v, double scalar);
double	vec2_dot(t_vec2 a, t_vec2 b);
double	vec2_length(t_vec2 v);
t_vec2	vec2_normalize(t_vec2 v);
t_vec2	vec2_multiply(t_vec2 a, t_vec2 b);
t_vec2	vec2_clamp(t_vec2 v, double min, double max);
double	vec2_distance(t_vec2 a, t_vec2 b);

#endif // VECLIB_H
