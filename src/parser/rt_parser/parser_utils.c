/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:47:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/19 17:58:28 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

void	print_parse_error(char *message, char *element, int line_nb)
{
	ft_putstr_fd("Error\nLine ", 2);
	ft_putnbr_fd(line_nb, 2);
	ft_putstr_fd(": ", 2);
	if (message)
		ft_putstr_fd(message, 2);
	if (element)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(element, 2);
	}
	if (message || element)
		ft_putstr_fd("\n", 2);
}

void	clear_gnl(int fd, char *line)
{
	if (line)
		free(line);
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

int	check_array_size(char **array, int expected, char *object, int line_nb)
{
	int	array_size;
	int	i;

	array_size = 0;
	while (array && array[array_size])
		(array_size)++;
	if (array_size < expected)
	{
		i = 0;
		print_parse_error("missing parameter(s)", object, line_nb);
		while (i < array_size)
		{
			ft_putstr_fd("\t", 2);
			ft_putstr_fd(array[i], 2);
			i++;
		}
		ft_putstr_fd("\n", 2);
	}
	return (array_size < expected);
}

static int	add_specials(t_object **dst, t_parser_ctx *ctx, char *elem)
{
	if (*dst)
	{
		print_parse_error("Duplicate element detected", elem, ctx->line_nb);
		return (0);
	}
	*dst = ctx->obj;
	return (1);
}

int	add_element_to_scene(t_scene *scene, t_parser_ctx *ctx)
{
	int		status;

	status = 1;
	if (!ctx->obj)
		return (1);
	if (ctx->obj->type >= OBJ_LIGHT)
	{
		if (ctx->obj->type == OBJ_LIGHT)
			ft_arrayadd_back(&scene->lights, ctx->obj, free_object);
		else
			ft_arrayadd_back(&scene->objects, ctx->obj, free_object);
		return (1);
	}
	if (ctx->obj->type == OBJ_AMBIENT)
		status = add_specials(&scene->ambient, ctx, "ambient");
	if (ctx->obj->type == OBJ_CAMERA)
		status = add_specials(&scene->cam, ctx, "camera");
	if (ctx->obj->type == OBJ_SKY)
		status = add_specials(&scene->sky, ctx, "sky");
	return (status);
}
