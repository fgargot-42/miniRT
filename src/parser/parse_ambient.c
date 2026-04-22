/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 19:08:43 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

int	parse_ambient(char **line_split, t_scene *scene, int line_nb)
{
	int		parse_result;
	double	intensity;

	parse_result = 1;
	if (scene->ambient)
	{
		print_parse_error("duplicate object detected", "ambient", line_nb);
		return (0);
	}
	if (check_array_size(line_split, 3, "ambient", line_nb))
		return (0);
	scene->ambient = malloc(sizeof(t_vec3));
	if (!scene->ambient)
	{
		print_parse_error("allocation failed", "ambient", line_nb);
		return (0);
	}
	parse_result = parse_vector(line_split[2], scene->ambient, "ambient",
			line_nb);
	parse_result &= parse_double(line_split[1], &intensity, "ambient", line_nb);
	if (parse_result)
		*scene->ambient = vec3_scale(*scene->ambient, intensity);
	return (parse_result);
}
