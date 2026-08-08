/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_draw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 19:01:55 by mabarrer          #+#    #+#             */
/*   Updated: 2026/08/08 01:24:43 by fgargot          ###   ########.fr       */
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
	*y += 8;
}

void	draw_ambient(t_data *d, int *y)
{
	draw_title(d, y, "AMBIENT -----");
	draw_slider_group(d, 11, 15, y);
}

void	draw_header(t_data *d)
{
	mlx_set_font_scale(d->mlx, "resources/font.ttf", 14.0f);
	mlx_string_put(d->mlx, d->editor, PANEL_X + PANEL_PAD, PANEL_Y + 16,
		(mlx_color){.rgba = COL_WHITE}, "miniRT INSPECTOR |      ;)");
}

void	draw_footer(t_data *d)
{
	draw_hline(d, d->editor, EDITOR_H - 4);
	mlx_set_font_scale(d->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(d->mlx, d->editor, PANEL_PAD, EDITOR_H - 4,
		(mlx_color){.rgba = COL_FOOTER}, "fgargot && mabarrer | miniRT");
	mlx_set_font_scale(d->mlx, "resources/font.ttf", 16.0f);
}
