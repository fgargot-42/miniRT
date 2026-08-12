/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:27:07 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/13 00:04:11 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "material.h"

void	apply_illum(t_material *mat)
{
	if (!mat)
		return ;
	if (mat->illum != 3)
		mat->reflectance = 0.0;
	if (mat->illum < 3)
	{
		mat->opacity = 1.0;
		mat->density = 1.0;
	}
}

static char	rt_tolower(unsigned int i, char c)
{
	(void)i;
	return ((char)ft_tolower(c));
}

int	strncmp_case_i(char const *str1, char const *str2, size_t n)
{
	char	*str1_i;
	char	*str2_i;
	int		cmp;

	str1_i = ft_strmapi(str1, rt_tolower);
	str2_i = ft_strmapi(str2, rt_tolower);
	if (str1_i && str2_i)
		cmp = ft_strncmp(str1_i, str2_i, n);
	else
		cmp = ft_strncmp(str1, str2, n);
	if (str1_i)
		free(str1_i);
	if (str2_i)
		free(str2_i);
	return (cmp);
}

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
