/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 19:36:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:36:10 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NORMAL_BONUS_H
# define NORMAL_BONUS_H

typedef struct s_bump
{
	t_object	*obj;
	t_vec2		uv;
	t_vec3		normal;
	double		cos_theta;
	double		sin_theta;
	double		cos_phi;
	double		sin_phi;
}	t_bump;

double	get_bump_from_img(t_vec2 uv, t_object obj);
t_vec3	bump_normal_sphere(t_hit_record rec, t_vec2 uv,
			double (*height_fn)(t_vec2, t_object));
t_vec3	bump_normal_triangle(t_hit_record rec, t_vec2 uv,
			double (*height_fn)(t_vec2, t_object));

#endif // NORMAL_BONUS_H
