/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 20:14:45 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_sphere_elements(char **line_split, t_sphere *sp, int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[1], &(sp->center), "sphere", line_nb);
	p_res = p_res && parse_double(line_split[2], &(sp->radius), "sphere",
			line_nb);
	p_res = p_res && parse_vector(line_split[3], &(sp->color), "sphere",
			line_nb);
	sp->specular = 0;
	sp->shininess = 1;
	if (line_split[4] && ft_strlen(line_split[4]))
		p_res = p_res && parse_double(line_split[4], &(sp->specular), "sphere",
				line_nb);
	if (line_split[4] && line_split[5] && ft_strlen(line_split[5]))
		p_res = p_res && parse_double(line_split[5], &(sp->shininess), "sphere",
				line_nb);
	if (p_res)
		sp->radius /= 2;
	return (p_res);
}

int	parse_sphere(char **line_split, t_scene *scene, int line_nb)
{
	int			parse_result;
	t_sphere	*sp;

	if (check_array_size(line_split, 4, "sphere", line_nb))
		return (0);
	sp = malloc(sizeof(t_sphere));
	if (!sp)
	{
		print_parse_error("allocation failed", "sphere", line_nb);
		return (0);
	}
	parse_result = parse_sphere_elements(line_split, sp, line_nb);
	ft_lstadd_back(&scene->spheres, ft_lstnew(sp));
	return (parse_result);
}
