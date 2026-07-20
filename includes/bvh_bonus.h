/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_bonus.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 22:03:29 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 22:05:22 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_BONUS_H
# define BVH_BONUS_H

# include "bvh.h"

void	get_cone_aabb(t_object *obj, t_aabb *aabb);
void	get_hyperboloid_aabb(t_object *obj, t_aabb *aabb);
void	get_paraboloid_aabb(t_object *obj, t_aabb *aabb);
void	get_triangle_aabb(t_object *obj, t_aabb *aabb);

#endif // BVH_BONUS_H
