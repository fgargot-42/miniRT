/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 22:30:46 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/08 02:03:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# define EDITOR_W       320
# define EDITOR_H       720
# define SLD_X          100
# define SLD_W          150
# define SLD_H          8
# define SLD_SPACING    17
# define SLD_POS_RANGE  50.0
# define SLD_BASE_Y     220
# define SLD_COL_BASE_Y 300
# define MAX_SLIDERS    32
# define MAX_BUTTONS    2

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
# define COL_BUTTON  	0x343456ff
# define COL_BTN_TEXT  	0xafafafff

# include "mlx.h"
# include "veclib.h"

typedef struct s_data	t_data;
typedef struct s_object	t_object;

typedef struct s_slider
{
	int			y;
	double		*value;
	double		min;
	double		max;
	const char	*label;
	mlx_color	col;
	bool		affects_bvh;
}	t_slider;

typedef struct s_button
{
	char	*label;
	t_vec2	pos;
	t_vec2	size;
	void	(*event)(void*);
}	t_button;

typedef struct s_row_info
{
	mlx_window	win;
	int			*y;
	const char	*label;
	mlx_color	val_col;
}	t_row_info;

typedef struct s_ui_info
{
	t_slider	sliders[MAX_SLIDERS];
	t_button	buttons[MAX_BUTTONS];
	int			nb_sliders;
	int			dragging_slider;
	int			selected_light;
}	t_ui_info;

// UI

void		init_editor(t_data *data);
void		setup_sliders(t_data *data);
void		draw_editor(t_data *data, double mouse_x, double mouse_y);
void		editor_mouse_down(int event, void *param);
void		editor_mouse_up(int event, void *param);
void		draw_header(t_data *d);
void		draw_footer(t_data *d);
void		draw_button(t_data *data, t_button *button);

void		fill_rect(t_data *data, t_vec2 pos, t_vec2 size,
				mlx_color col);

void		draw_hline(t_data *data, void *win, int y);

void		put_row(t_data *data, t_row_info info, const char *str);
void		put_section(t_data *data, void *win, int *y,
				const char *title);
void		setup_transform_sliders(t_data *data, t_object *obj);
void		setup_color_sliders(t_data *data, t_object *obj);
void		setup_property_sliders(t_data *data, t_object *obj);

void		setup_ambient_sliders(t_data *data, int slider_id);
mlx_color	slider_dim(t_slider *s);
double		slider_ratio(t_slider *s);
void		draw_object_header(t_data *data, double mouse_x, double mouse_y);
void		draw_group(t_data *d, t_vec2 range, int *y, char *title);
void		draw_ambient(t_data *d, int *y);
void		draw_slider_group(t_data *d, int start, int end, int *y);
const char	*obj_type_name(t_object *o);

void		setup_button_event(t_button *button, char *label, t_vec2 pos,
				void (*event)(void *));
void		select_prev_light(void *data);
void		select_next_light(void *data);

#endif //UI_H
