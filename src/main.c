#include <mlx.h>
#include <stdlib.h>
#include "miniRT.h"

#define T_MIN 0.001
#define T_MAX 1e9


mlx_color   vec3_to_color(t_vec3 v)
{
    return (mlx_color){
        .r = (int)v.x,
        .g = (int)v.y,
        .b = (int)v.z,
        .a = 255
    };
}
void	init(t_data *data)
{
	mlx_window_create_info	info;

	data->mlx = mlx_init();
	if (!data->mlx)
		exit(1);
	info = (mlx_window_create_info)
	{
		.title = "miniRT",
		.width = WIDTH,
		.height = HEIGHT,
	};
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		exit(1);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		exit(1);
}

void    init_scene(t_scene *scene)
{
    t_sphere    *s;

    scene->cam.position = (t_vec3){0, 0, 0};
    scene->cam.fov = 90;

    scene->light.position = (t_vec3){-3, 5, 2};
    scene->light.intensity = 1.0;
    scene->light.color = (t_vec3){255, 255, 255};

    scene->ambient = (t_vec3){20, 20, 20};

    s = malloc(sizeof(t_sphere));
    s->center = (t_vec3){0, 0, 5};
    s->radius = 1.0;
    s->color = (t_vec3){149, 215, 174};
    ft_lstadd_back(&scene->spheres, ft_lstnew(s));
}


void	draw(t_data *data)
{
	t_sphere		sphere[3];
	t_plane			plane;
	t_camera		cam;
	t_ray			r;
	t_hit_record	hc;
	t_hit_record	closest;
	int				x;
	int				y;
	int				i;
	int				hit;

	plane.color = (t_vec3){69, 72, 81};
	plane.point = (t_vec3){0, -3, 0};
	plane.normal = (t_vec3){0, 1, 0};
	cam.position = (t_vec3){0, 0, 0};
	cam.fov = 90;
	sphere[0].center = (t_vec3){0, 0, 5};
	sphere[0].color = (t_vec3){115, 149, 111};
	sphere[0].radius = 1;
	sphere[1].center = (t_vec3){1, -.5, 3};
	sphere[1].color = (t_vec3){123, 174, 127};
	sphere[1].radius = 0.8;
	sphere[2].center = (t_vec3){-1, .5, 3};
	sphere[2].color = (t_vec3){149, 215, 174};
	sphere[2].radius = 1.1;
	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			r = camera_ray(cam, x, y);
			closest.t = T_MAX;
			hit = 0;
			i = 0;
			while (i < 3)
			{
				if (hit_sphere(&sphere[i], &r, T_MIN, closest.t, &hc))
				{
					closest = hc;
					hit = 1;
				}
				i++;
			}
			if (hit_plane(&plane, &r, T_MIN, closest.t, &hc))
			{
				closest = hc;
				hit = 1;
			}
			if (hit)
				mlx_set_image_pixel(data->mlx, data->img, x, y, vec3_to_color(closest.color));
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int hit_scene(t_scene *scene, t_ray *ray, double t_min, double t_max, t_hit_record *rec)
{
    t_hit_record    temp_rec;
    int            hit_anything;
    double         closest;
    t_list         *current;
    
    hit_anything = 0;
    closest = t_max;
    
    current = scene->spheres;
    while (current)
    {
        if (hit_sphere(current->content, ray, t_min, closest, &temp_rec))
        {
            hit_anything = 1;
            closest = temp_rec.t;
            *rec = temp_rec;
        }
        current = current->next;
    }
    
    current = scene->planes;
    while (current)
    {
        if (hit_plane(current->content, ray, t_min, closest, &temp_rec))
        {
            hit_anything = 1;
            closest = temp_rec.t;
            *rec = temp_rec;
        }
        current = current->next;
    }
    
/*    current = scene->cylinders;
    while (current)
    {
        if (hit_cylinder(current->content, ray, t_min, closest, &temp_rec))
        {
            hit_anything = 1;
            closest = temp_rec.t;
            *rec = temp_rec;
        }
        current = current->next;
    }*/
    
    return (hit_anything);
}
static void	destroy_all(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_context(data->mlx);
}

int	main(void)
{
	t_data	data;

	init(&data);
	draw(&data);
	attach_hooks(&data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
