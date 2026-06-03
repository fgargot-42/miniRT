/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:27:07 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/01 22:45:13 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "material.h"

int	parse_new_material(char *line, t_list **mat_list, t_material **mat)
{
	char	**split;

	if (!line || !mat_list || !mat)
		return (0);
	*mat = ft_calloc(1, sizeof(t_material));
	if (*mat)
	{
		split = ft_split_by_whitespace(line);
		if (!split || !split[0] || !split[1])
		{
			if (split)
				free_str_array(split);
			free(*mat);
			*mat = NULL;
			return (0);
		}
		(*mat)->name = ft_strdup(split[1]);
		free_str_array(split);
	}
	if (*mat)
		ft_lstadd_back(mat_list, ft_lstnew(*mat));
	return (*mat != NULL);
}

int	parse_mat_exponent(char *line, t_material *mat, int line_nb)
{
	char	**split;
	int		parse_result;

	if (!line || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 2, "Ns", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &mat->specular, "Ns", line_nb);
	free_str_array(split);
	return (parse_result);
}

int	parse_mat_ambient(char *line, t_material *mat, int line_nb)
{
	char	**split;
	int		parse_result;

	if (!line || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 4, "Ka", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &mat->amb_color.x, "Ka", line_nb);
	parse_result &= parse_double(split[2], &mat->amb_color.y, "Ka", line_nb);
	parse_result &= parse_double(split[3], &mat->amb_color.z, "Ka", line_nb);
	free_str_array(split);
	return (parse_result);
}

int	parse_mat_diffuse(char *line, t_material *mat, int line_nb)
{
	char	**split;
	int		parse_result;

	if (!line || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 4, "Kd", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &mat->diff_color.x, "Kd", line_nb);
	parse_result &= parse_double(split[2], &mat->diff_color.y, "Kd", line_nb);
	parse_result &= parse_double(split[3], &mat->diff_color.z, "Kd", line_nb);
	free_str_array(split);
	return (parse_result);
}

int	parse_mat_specular(char *line, t_material *mat, int line_nb)
{
	char	**split;
	int		parse_result;

	if (!line || !mat)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 4, "Ks", line_nb))
	{
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &mat->spec_color.x, "Ks", line_nb);
	parse_result &= parse_double(split[2], &mat->spec_color.y, "Ks", line_nb);
	parse_result &= parse_double(split[3], &mat->spec_color.z, "Ks", line_nb);
	free_str_array(split);
	return (parse_result);
}
