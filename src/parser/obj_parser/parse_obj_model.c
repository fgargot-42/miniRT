/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj_model.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:18:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/05 21:45:04 by fgargot          ###   ########.fr       */
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

int	parse_vertex(char *line, t_vec3 ***vertices, int line_nb)
{
	char	**split;
	int		v_count;
	int		parse_result;
	t_vec3	*vertex;

	if (!vertices)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	v_count = 0;
	while (*vertices && (*vertices)[v_count])
		v_count++;
	vertex = ft_calloc(1, sizeof(t_vec3));
	*vertices = realloc(*vertices, sizeof(t_vec3 *) * (v_count + 2));
	if (!*vertices || !vertex || check_array_size(split, 4, "v", line_nb))
	{
		if (vertex)
			free(vertex);
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &vertex->x, "v", line_nb);
	parse_result &= parse_double(split[2], &vertex->y, "v", line_nb);
	parse_result &= parse_double(split[3], &vertex->z, "v", line_nb);
	parse_result &= apply_vertex_scale(split[4], vertex);
	free_str_array(split);
	*(*vertices + v_count) = vertex;
	*(*vertices + v_count + 1) = NULL;
	return (parse_result);
}

int	parse_normal(char *line, t_vec3 ***normal_array, int line_nb)
{
	char	**split;
	int		normal_count;
	int		parse_result;
	t_vec3	*normal;

	if (!normal_array)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	normal_count = 0;
	while (*normal_array && (*normal_array)[normal_count])
		normal_count++;
	normal = ft_calloc(1, sizeof(t_vec3));
	*normal_array = realloc(*normal_array, sizeof(t_vec3 *) * (normal_count + 2));
	if (!*normal_array || !normal || check_array_size(split, 4, "vn", line_nb))
	{
		if (normal)
			free(normal);
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &normal->x, "vn", line_nb);
	parse_result &= parse_double(split[2], &normal->y, "vn", line_nb);
	parse_result &= parse_double(split[3], &normal->z, "vn", line_nb);
	free_str_array(split);
	*(*normal_array + normal_count) = normal;
	*(*normal_array + normal_count + 1) = NULL;
	return (parse_result);
}

int	parse_texture(char *line, t_vec2 ***texture_array, int line_nb)
{
	char	**split;
	int		tex_count;
	int		parse_result;
	t_vec2	*texture;

	if (!texture_array)
		return (0);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	tex_count = 0;
	while (*texture_array && (*texture_array)[tex_count])
		tex_count++;
	*texture_array = realloc(*texture_array, sizeof(t_vec2 *) * (tex_count + 2));
	texture = ft_calloc(1, sizeof(t_vec2));
	if (!(*texture_array) || !texture || check_array_size(split, 2, "vt", line_nb))
	{
		if (texture)
			free(texture);
		free_str_array(split);
		return (0);
	}
	parse_result = parse_double(split[1], &texture->x, "vt", line_nb);
	if (split[2])
		parse_result &= parse_double(split[2], &texture->y, "vt", line_nb);
	free_str_array(split);
	*(*texture_array + tex_count) = texture;
	*(*texture_array + tex_count + 1) = NULL;
	return (parse_result);
}
