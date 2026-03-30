#include "miniRT.h"
#include <SDL2/SDL_scancode.h>
void	window_hook(int event, void *param)
{
	mlx_context	mlx;

	(void)event;
	mlx = (mlx_context)param;
	if (event == 0)
		mlx_loop_end(mlx);
}

void	key_hook(int keycode, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keycode == SDL_SCANCODE_ESCAPE)
		mlx_loop_end(data->mlx);
}

void	attach_hooks(t_data *data)
{
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook, data->mlx);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_hook, data);
}

