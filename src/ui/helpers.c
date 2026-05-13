/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:07:21 by mabarrer          #+#    #+#             */
/*   Updated: 2026/05/13 20:07:23 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "ui.h"

void	fill_rect(t_data *data, t_vec2 pos, t_vec2 size, mlx_color col)
{
	int	px;
	int	py;

	py = pos.y;
	while (py < pos.y + size.y)
	{
		px = pos.x;
		while (px < pos.x + size.x)
			mlx_pixel_put(data->mlx, data->editor, px++, py, col);
		py++;
	}
}

void	draw_hline(t_data *data, void *win, int y)
{
	int	x;

	x = PANEL_X + PANEL_PAD;
	while (x < PANEL_X + PANEL_W - PANEL_PAD)
		mlx_pixel_put(data->mlx, win, x++, y, (mlx_color){.rgba = COL_SEP});
}

void	put_row(t_data *data, void *win, int *y,
		const char *label, mlx_color val_col, const char *fmt, ...)
{
	char	buf[128];
	va_list	args;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(data->mlx, win, PANEL_X + PANEL_PAD, *y,
		(mlx_color){.rgba = COL_LABEL}, (char *)label);
	mlx_string_put(data->mlx, win, (PANEL_X + PANEL_W / 2), *y, val_col, buf);
	*y += LINE_H;
}

void	put_section(t_data *data, void *win, int *y, const char *title)
{
	*y += 12;
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(data->mlx, win, PANEL_X + PANEL_PAD, *y,
		(mlx_color){.rgba = COL_SECTION}, (char *)title);
	*y += LINE_H - 6;
	draw_hline(data, win, *y);
	*y += 16;
}
