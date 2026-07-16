/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 16:30:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 22:58:59 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HIT_H
# define HIT_H
# include "veclib.h"

typedef struct s_hit_ctx
{
	double	t_max;
	union
	{
		struct
		{
			t_vec3	oc;
			t_vec3	rd;
		};
		t_ray	obj_ray;
	};
	double	render_t;
	t_vec3	render_hit;
}	t_hit_ctx;

#endif	//HIT_H
