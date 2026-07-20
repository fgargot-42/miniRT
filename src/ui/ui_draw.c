/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_draw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 19:01:55 by mabarrer          #+#    #+#             */
/*   Updated: 2026/07/20 19:02:40 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_title(t_data *d, int *y, char *title)
{
	put_section(d, d->editor, y, title);
}

void	draw_group(t_data *d, t_vec2 range, int *y, char *title)
{
	draw_title(d, y, title);
	draw_slider_group(d, range.x, range.y, y);
	*y += 12;
}

void	draw_ambient(t_data *d, int *y)
{
	draw_title(d, y, "AMBIENT -----");
	draw_slider_group(d, 11, 15, y);
}
