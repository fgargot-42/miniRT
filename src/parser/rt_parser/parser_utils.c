/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 17:47:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/27 00:21:59 by fgargot          ###   ########.fr       */
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
	t_list	*new_object;

	status = 1;
	if (!ctx->obj)
		return (1);
	if (ctx->obj->type >= OBJ_LIGHT)
	{
		new_object = ft_lstnew(ctx->obj);
		if (!new_object)
			return (0);
		if (ctx->obj->type == OBJ_LIGHT)
			ft_lstadd_back(&scene->lights, new_object);
		else
			ft_lstadd_back(&scene->objects, new_object);
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
