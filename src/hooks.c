#include "miniRT.h"
#include <SDL2/SDL_scancode.h>

#define MOVE_STEP 0.5


void	window_hook(int event, void *param)
{
	mlx_context	mlx;

	(void)event;
	mlx = (mlx_context)param;
	if (event == 0)
		mlx_loop_end(mlx);
}

void key_hook(int scancode, void *param)
{
    t_data *data = (t_data *)param;
    t_camera *cam = &data->scene->cam;

    // FPS-style movement
    t_vec3 forward = vec_normalize(cam->direction);
    t_vec3 world_up = (t_vec3){0, 1, 0};
    t_vec3 right = vec_normalize(vec_cross(forward, world_up));

    // Exit
	printf("code: %d\n", scancode);

    if (scancode == SDL_SCANCODE_ESCAPE)
        mlx_loop_end(data->mlx);

    // Move forward/back

    if (scancode == 26)
        cam->position = vec_add(cam->position, vec_scale(forward, MOVE_STEP));
    else if (scancode == 22)
        cam->position = vec_add(cam->position, vec_scale(forward, -MOVE_STEP));

    // Strafe left/right
    else if (scancode == 7)
        cam->position = vec_add(cam->position, vec_scale(right, MOVE_STEP));
    else if (scancode == 4)
        cam->position = vec_add(cam->position, vec_scale(right, -MOVE_STEP));

    // Move up/down
    else if (scancode == SDL_SCANCODE_E)
        cam->position = vec_add(cam->position, vec_scale(world_up, MOVE_STEP));
    else if (scancode == SDL_SCANCODE_Q)
        cam->position = vec_add(cam->position, vec_scale(world_up, -MOVE_STEP));

    printf("Camera position: x=%.2f y=%.2f z=%.2f\n",
           cam->position.x, cam->position.y, cam->position.z);
    // Redraw the scene
    draw(data);
}

void	attach_hooks(t_data *data)
{
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook, data->mlx);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_hook, data);
}

