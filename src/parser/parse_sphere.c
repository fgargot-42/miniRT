/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 22:04:43 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_sphere_elements(char **line_split, t_object *obj, int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(obj->position), "sphere", line_nb);
	p_res = p_res && parse_double(line_split[2], &(obj->radius), "sphere",
			line_nb);
	p_res = p_res && parse_vector(line_split[3], &(obj->color), "sphere",
			line_nb);
	if (line_split[4] && ft_strlen(line_split[4]))
		p_res = p_res && parse_double(line_split[4], &(obj->specular), "sphere",
				line_nb);
	if (line_split[4] && line_split[5] && ft_strlen(line_split[5]))
		p_res = p_res && parse_double(line_split[5], &(obj->shininess), "sphere",
				line_nb);
	return (p_res);
}

t_object	*parse_sphere(char **line_split, int line_nb)
{
	int			parse_result;
	t_object	*obj;

	if (check_array_size(line_split, 4, "sphere", line_nb))
		return (0);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "sphere", line_nb);
		return (0);
	}
	obj->shininess = 1;
	parse_result = parse_sphere_elements(line_split, obj, line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->radius /= 2;
	obj->direction = (t_vec3){0, 0, 1};
	obj->type = OBJ_SPHERE;
	return (obj);
}
