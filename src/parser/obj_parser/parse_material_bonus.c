/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:27:07 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/30 19:14:13 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "material.h"

static void	init_material_props(t_material *m, char *name)
{
	m->density = 1.0;
	m->opacity = 1.0;
	m->shininess = 32.0;
	m->illum = 2;
	m->name = ft_strdup(name);
}

int	parse_new_material(char *line, t_array *materials, void *mat_ptr)
{
	char		**split;
	t_material	**mat;

	mat = mat_ptr;
	if (!line || !materials || !mat)
		return (0);
	*mat = ft_calloc(1, sizeof(t_material));
	if (!*mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split || !split[0] || !split[1])
	{
		if (split)
			free_str_array(split);
		free(*mat);
		*mat = NULL;
		return (0);
	}
	init_material_props(*mat, split[1]);
	free_str_array(split);
	ft_arrayadd_back(materials, *mat, free);
	return (1);
}

void	destroy_texture(t_texture **t)
{
	if (!*t)
		return ;
	mlx_destroy_image((*t)->mlx, (*t)->data);
	free(*t);
	*t = NULL;
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
