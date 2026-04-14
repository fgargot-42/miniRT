/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sky.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/14 22:52:00 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

int	parse_sky(char **line_split, t_scene *scene, int line_nb)
{
	int		parse_result;

	if (scene->sky)
	{
		print_parse_error("duplicate object detected", "sky", line_nb);
		return (0);
	}
	if (check_array_size(line_split, 2, "sky", line_nb))
		return (0);
	scene->sky = malloc(sizeof(t_vec3));
	if (!scene->sky)
	{
		print_parse_error("allocation failed", "sky", line_nb);
		return (0);
	}
	parse_result = parse_vector(line_split[1], scene->sky, "sky", line_nb);
	return (parse_result);
}
