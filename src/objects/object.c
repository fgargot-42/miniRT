/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 19:34:17 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/24 19:36:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdlib.h>

t_object	*create_object(void *object, t_obj_type type)
{
	t_object *o;

	o = malloc(sizeof(t_object));
	if (!o)
		return (NULL);
	o->object = object;
	o->type = type;
	return (o);
}
