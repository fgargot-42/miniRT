/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_bounds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:13:32 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/12 22:59:55 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static void	update_object_min_max(t_object *obj, t_object *current)
{
	obj->props.min = (t_vec3){
		fmin(fmin(fmin(current->props.a.x, current->props.b.x),
				current->props.c.x), obj->props.min.x),
		fmin(fmin(fmin(current->props.a.y, current->props.b.y),
				current->props.c.y), obj->props.min.y),
		fmin(fmin(fmin(current->props.a.z, current->props.b.z),
				current->props.c.z), obj->props.min.z)};
	obj->props.max = (t_vec3){
		fmax(fmax(fmax(current->props.a.x, current->props.b.x),
				current->props.c.x), obj->props.max.x),
		fmax(fmax(fmax(current->props.a.y, current->props.b.y),
				current->props.c.y), obj->props.max.y),
		fmax(fmax(fmax(current->props.a.z, current->props.b.z),
				current->props.c.z), obj->props.max.z)};
}

void	update_box_min_max(t_object *obj)
{
	t_list		*current;
	t_object	*content;

	current = obj->props.triangles;
	if (!current)
		return ;
	content = (t_object *)(current)->content;
	obj->props.min = (t_vec3){
		fmin(fmin(content->props.a.x, content->props.b.x), content->props.c.x),
		fmin(fmin(content->props.a.y, content->props.b.y), content->props.c.y),
		fmin(fmin(content->props.a.z, content->props.b.z), content->props.c.z)};
	obj->props.max = (t_vec3){
		fmax(fmax(content->props.a.x, content->props.b.x), content->props.c.x),
		fmax(fmax(content->props.a.y, content->props.b.y), content->props.c.y),
		fmax(fmax(content->props.a.z, content->props.b.z), content->props.c.z)};
	current = current->next;
	while (current)
	{
		update_object_min_max(obj, (t_object *)(current)->content);
		current = current->next;
	}
}
