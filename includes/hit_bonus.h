/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_bonus.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 23:03:19 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 23:06:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HIT_BONUS_H
# define HIT_BONUS_H

# include "hit.h"

int		hit_cone(t_object *obj, t_ray *ray, double t_max, t_hit_record *rec);
int		hit_hyperboloid(t_object *obj, t_ray *ray, double t_max,
			t_hit_record *rec);
int		hit_paraboloid(t_object *obj, t_ray *ray, double t_max,
			t_hit_record *rec);
int		hit_triangle(t_object *obj, t_ray *ray, double t_max,
			t_hit_record *rec);
void	apply_checker(t_hit_record *rec, t_object *obj, t_vec3 point);

#endif // HIT_BONUS_H
