/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj_model.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:18:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/19 22:42:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object.h"
#include "miniRT.h"
#include "veclib.h"
#include "libft.h"

static int	apply_vertex_scale(char *scale_str, t_vec3 *vertex)
{
	double	scale;

	if (!scale_str)
		return (1);
	if (!is_numeric(scale_str))
		return (1);
	scale = ft_atod(scale_str);
	if (fabs(scale) > 1e-10)
		*vertex = vec3_scale(*vertex, scale);
	return (1);
}

bool	parse_vertex(char *line, t_array *vertices, int line_nb)
{
	char	**split;
	bool	parse_result;
	t_vec3	*vertex;

	if (!vertices)
		return (false);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (false);
	vertex = ft_calloc(1, sizeof(t_vec3));
	if (!vertex || check_array_size(split, 4, "v", line_nb))
	{
		if (vertex)
			free(vertex);
		free_str_array(split);
		return (false);
	}
	parse_result = parse_double(split[1], &vertex->x, "v", line_nb);
	parse_result &= parse_double(split[2], &vertex->y, "v", line_nb);
	parse_result &= parse_double(split[3], &vertex->z, "v", line_nb);
	parse_result &= apply_vertex_scale(split[4], vertex);
	free_str_array(split);
	ft_arrayadd_back(vertices, vertex, free);
	return (parse_result);
}

bool	parse_normal(char *line, t_array *normal_array, int line_nb)
{
	char	**split;
	bool	parse_result;
	t_vec3	*normal;

	if (!normal_array)
		return (false);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (false);
	normal = ft_calloc(1, sizeof(t_vec3));
	if (!normal || check_array_size(split, 4, "vn", line_nb))
	{
		if (normal)
			free(normal);
		free_str_array(split);
		return (false);
	}
	parse_result = parse_double(split[1], &normal->x, "vn", line_nb);
	parse_result &= parse_double(split[2], &normal->y, "vn", line_nb);
	parse_result &= parse_double(split[3], &normal->z, "vn", line_nb);
	free_str_array(split);
	ft_arrayadd_back(normal_array, normal, free);
	return (parse_result);
}

bool	parse_texture(char *line, t_array *texture_array, int line_nb)
{
	char	**split;
	bool	parse_result;
	t_vec2	*texture;

	if (!texture_array)
		return (false);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (false);
	texture = ft_calloc(1, sizeof(t_vec2));
	if (!texture || check_array_size(split, 2, "vt", line_nb))
	{
		if (texture)
			free(texture);
		free_str_array(split);
		return (false);
	}
	parse_result = parse_double(split[1], &texture->x, "vt", line_nb);
	if (split[2])
		parse_result &= parse_double(split[2], &texture->y, "vt", line_nb);
	free_str_array(split);
	ft_arrayadd_back(texture_array, texture, free);
	return (parse_result);
}
