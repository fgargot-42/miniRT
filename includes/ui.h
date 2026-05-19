/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 22:30:46 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/19 18:45:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# define MAX_SLIDERS    17
# define EDITOR_W       430
# define EDITOR_H       1000
# define SLD_X          100
# define SLD_W          280
# define SLD_H          10
# define SLD_SPACING    26
# define SLD_POS_RANGE  50.0
# define SLD_BASE_Y     320
# define SLD_COL_BASE_Y 430


#define PANEL_X       0
#define PANEL_Y       0
#define PANEL_W       430
#define PANEL_PAD     10
#define LINE_H        20
#define TITLE_H       22

#define COL_BG        	0x1e1e2eff
#define COL_TITLEBAR  	0x2d4a7aff
#define COL_BORDER		0x4a6fa5ff
#define COL_LABEL     	0x708090ff
#define COL_VALUE     	0xf0d060ff
#define COL_ADDR      	0x64b4e8ff
#define COL_TYPE      	0x78e878ff
#define COL_SECTION   	0x5a8a5aff
#define COL_SEP       	0x2a4a2aff
#define COL_FOOTER    	0x405060ff
#define COL_WHITE     	0xffffffff
#define COL_TRACK_BG  	0x12121eff

# include "mlx.h"

typedef struct s_slider
{
	int			y;
	double		*value;
	double		min;
	double		max;
	const char	*label;
	mlx_color	col;
}	t_slider;
#endif //UI_H
