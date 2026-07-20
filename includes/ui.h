/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 22:30:46 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 23:22:01 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# define EDITOR_W       300
# define EDITOR_H       667
# define SLD_X          90
# define SLD_W          150
# define SLD_H          5
# define SLD_SPACING    17
# define SLD_POS_RANGE  50.0
# define SLD_BASE_Y     220
# define SLD_COL_BASE_Y 300

# define PANEL_X       0
# define PANEL_Y       0
# define PANEL_W       300
# define PANEL_PAD     8
# define LINE_H        14
# define TITLE_H       16

# define COL_BG        	0x1e1e2eff
# define COL_TITLEBAR  	0x2d4a7aff
# define COL_BORDER		0x4a6fa5ff
# define COL_LABEL     	0x708090ff
# define COL_VALUE     	0xf0d060ff
# define COL_ADDR      	0x64b4e8ff
# define COL_TYPE      	0x78e878ff
# define COL_SECTION   	0x5a8a5aff
# define COL_SEP       	0x2a4a2aff
# define COL_FOOTER    	0x405060ff
# define COL_WHITE     	0xffffffff
# define COL_TRACK_BG  	0x12121eff

# include "mlx.h"
# include "miniRT.h"
# include "types.h"
mlx_color	slider_dim(t_slider *s);
double	slider_ratio(t_slider *s);
void	header(t_data *data, double mouse_x, double mouse_y);
void	draw_group(t_data *d, t_vec2 range, int *y, char *title);
void	draw_ambient(t_data *d, int *y);
void	draw_slider_group(t_data *d, int start, int end, int *y);
const char	*obj_type_name(t_object *o);

#endif //UI_H
