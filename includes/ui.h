#ifndef UI_H
#define UI_H

# define MAX_SLIDERS    10
# define EDITOR_W       430
# define EDITOR_H       800
# define SLD_X          100
# define SLD_W          280
# define SLD_H          10
# define SLD_SPACING    26
# define SLD_POS_RANGE  50.0
# define SLD_BASE_Y     320
# define SLD_COL_BASE_Y 430

#include "mlx.h"
typedef struct	s_slider
{
	int	y;
	double	*value;
	double	min;
	double	max;
	const char *label;
	mlx_color col;
}	t_slider;
#endif
