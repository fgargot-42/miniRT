/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 00:05:08 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_camera_elements(char **line_split, t_camera *cam, int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(cam->position), "camera", line_nb);
	p_res &= parse_vector(line_split[2], &(cam->direction), "camera", line_nb);
	p_res &= parse_double(line_split[3], &(cam->fov), "camera", line_nb);
	return (p_res);
}

t_object	*parse_camera(char **line_split, int line_nb)
{
	int			parse_result;
	t_camera	*cam;
	t_object	*obj;

	if (check_array_size(line_split, 4, "camera", line_nb))
		return (0);
	cam = malloc(sizeof(t_camera));
	if (!cam)
	{
		print_parse_error("allocation failed", "camera", line_nb);
		return (0);
	}
	parse_result = parse_camera_elements(line_split, cam, line_nb);
	if (!parse_result)
	{
		free(cam);
		return (NULL);
	}
	cam->pitch = 0.0;
	cam->yaw = 0.0;
	obj = create_object(cam, OBJ_CAMERA);
	if (!obj)
		free(cam);
	return (obj);
}
