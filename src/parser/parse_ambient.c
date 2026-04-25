/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/24 23:58:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	apply_ambient_intensity(t_ambient *ambient)
{
	ambient->color = vec3_scale(ambient->color, ambient->intensity);
}

static int	parse_ambient_elements(char **line_split, t_ambient *amb,
	int line_nb)
{
	int	p_res;

	
	p_res = parse_vector(line_split[2], &amb->color, "ambient", line_nb);
	p_res &= parse_double(line_split[1], &amb->intensity, "ambient", line_nb);
	if (p_res)
		apply_ambient_intensity(amb);
	return (p_res);
}

t_object	*parse_ambient(char **line_split, int line_nb)
{
	int			parse_result;
	t_ambient	*amb;
	t_object	*obj;

	obj = NULL;
	parse_result = 1;
	if (check_array_size(line_split, 3, "ambient", line_nb))
		return (NULL);
	amb = malloc(sizeof(t_ambient));
	if (!amb)
	{
		print_parse_error("allocation failed", "ambient", line_nb);
		return (NULL);
	}
	parse_result = parse_ambient_elements(line_split, amb, line_nb);
	if (parse_result)
		obj = create_object(amb, OBJ_AMBIENT);
	if (!obj)
		free(amb);
	return (obj);
}
