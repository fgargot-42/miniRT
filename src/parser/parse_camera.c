/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 23:43:18 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

int	parse_camera(char **line_split, t_scene *scene, int line_nb)
{
	int			parse_result;

	if (scene->cam)
	{
		print_parse_error("duplicate object detected", "camera", line_nb);
		return (0);
	}
	if (check_array_size(line_split, 4, "camera", line_nb))
		return (0);
	scene->cam = malloc(sizeof(t_camera));
	if (!scene->cam)
	{
		print_parse_error("allocation failed", "camera", line_nb);
		return (0);
	}
	parse_result = parse_vector(line_split[1], &(scene->cam->position),
			"camera", line_nb);
	parse_result &= parse_vector(line_split[2], &(scene->cam->direction),
			"camera", line_nb);
	parse_result &= parse_double(line_split[3], &(scene->cam->fov), "camera",
			line_nb);
	scene->cam->pitch = 0.0;
	scene->cam->yaw = 0.0;
	return (parse_result);
}
