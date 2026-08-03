/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 18:30:04 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 19:29:13 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "libft.h"
#include <stdlib.h>

static void	init_material_props(t_material *m, char *name)
{
	m->density = 1.0;
	m->opacity = 1.0;
	m->shininess = 32.0;
	m->illum = 2;
	m->name = ft_strdup(name);
}

t_material	*new_material(char *name)
{
	t_material	*new_mat;

	new_mat = ft_calloc(1, sizeof(t_material));
	if (new_mat)
		init_material_props(new_mat, name);
	return (new_mat);
}

void	destroy_material(void *o)
{
	t_material	*mat;

	mat = (t_material *)o;
	if (mat->name)
		free(mat->name);
	destroy_texture(&mat->color_tex);
	destroy_texture(&mat->normal_tex);
	destroy_texture(&mat->spec_tex);
	destroy_texture(&mat->mask_tex);
	free(mat);
}
