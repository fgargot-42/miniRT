/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 20:27:07 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/09 19:53:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
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
	parse_result = parse_double(split[1], &mat->shininess, "Ns", line_nb);
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
	parse_result = parse_double(split[1], &mat->ambient.x, "Ka", line_nb);
	parse_result &= parse_double(split[2], &mat->ambient.y, "Ka", line_nb);
	parse_result &= parse_double(split[3], &mat->ambient.z, "Ka", line_nb);
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
	parse_result = parse_double(split[1], &mat->diffuse.x, "Kd", line_nb);
	parse_result &= parse_double(split[2], &mat->diffuse.y, "Kd", line_nb);
	parse_result &= parse_double(split[3], &mat->diffuse.z, "Kd", line_nb);
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
	parse_result = parse_double(split[1], &mat->specular.x, "Ks", line_nb);
	parse_result &= parse_double(split[2], &mat->specular.y, "Ks", line_nb);
	parse_result &= parse_double(split[3], &mat->specular.z, "Ks", line_nb);
	free_str_array(split);
	return (parse_result);
}
