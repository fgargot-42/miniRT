/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:27:07 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 19:10:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "material.h"

int	parse_new_material(char *line, t_array *materials, void *mat_ptr)
{
	char		**split;
	t_material	**mat;

	mat = mat_ptr;
	if (!line || !materials || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split || !split[0] || !split[1])
	{
		if (split)
			free_str_array(split);
		return (0);
	}
	*mat = new_material(split[1]);
	free_str_array(split);
	if (!*mat)
		return (0);
	ft_arrayadd_back(materials, *mat, free);
	return (1);
}
