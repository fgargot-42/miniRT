/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 21:16:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/08 00:35:43 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "material.h"

int	parse_mat_emissive(char *line, t_material *mat, int line_nb)
{
	char	**split;
	int		parse_result;

	if (!line || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 4, "Ke", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &mat->emissive.x, "Ke", line_nb);
	parse_result &= parse_double(split[2], &mat->emissive.y, "Ke", line_nb);
	parse_result &= parse_double(split[3], &mat->emissive.z, "Ke", line_nb);
	free_str_array(split);
	return (parse_result);
}

int	parse_mat_density(char *line, t_material *mat, int line_nb)
{
	char	**split;
	int		parse_result;

	if (!line || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (!split[0] || !split[1])
	if (check_array_size(split, 2, "Ni", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &mat->density, "Ni", line_nb);
	free_str_array(split);
	return (parse_result);
}

int	parse_mat_opacity(char *line, t_material *mat, int line_nb)
{
	char	**split;
	int		parse_result;

	if (!line || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (!split[0] || !split[1])
	if (check_array_size(split, 2, "d", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &mat->opacity, "d", line_nb);
	free_str_array(split);
	return (parse_result);
}
