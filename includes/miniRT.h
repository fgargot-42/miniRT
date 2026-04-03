#include "veclib.h"
#include "mlx.h"
#include "math.h"
#include "SDL2/SDL_scancode.h"
#define T_MIN 0.001
#define T_MAX 100

#include "libft.h"

#define WIDTH 1280
#define HEIGHT 720
#define CAMERA_SENS 0.35

#define NB_THREADS 16

typedef struct s_ray
{
    t_vec3  origin;
    t_vec3  direction;
}   t_ray;


// P(t) = O + t * D
// t distance sur le ray
// O origin
// D direction

typedef struct s_sphere
{
	t_vec3 center;
	double radius;
	t_vec3 color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3 point;
	t_vec3 normal; // faut normaliser
	t_vec3 color;
	int	checker;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3 center;
	t_vec3 axis;
	double radius;
	double height;
	t_vec3 color;
}	t_cylinder;

typedef struct s_camera
{
	t_vec3 position;
	t_vec3 direction;
	double fov;
	double	pitch;
	double	yaw;
}	t_camera;
typedef union u_object
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
}	t_object;

typedef struct s_hit_record
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	object;
}	t_hit_record;

typedef struct s_hit_list
{
	t_list *list;
	int		(*hit_fn)(void *, t_ray *, double, double, t_hit_record *);
}	t_hit_list;

typedef struct s_light
{
	t_vec3 position;
	double intensity;
	t_vec3 color;
}	t_light;

typedef struct s_scene
{
	t_list *spheres;
	t_list *planes;
	t_list *cylinder;
	t_light light;
	t_vec3 ambient;
	t_camera cam;
}	t_scene;

typedef struct s_data
{
	mlx_context	mlx;
	mlx_window	win;
	mlx_image	img;
	t_scene		*scene;

	int			last_mouse_x;
	int			last_mouse_y;

	int			render_scale;
	int			th_nb;
	int			nb_threads;
}	t_data;

//src/hooks.c
void	attach_hooks(t_data *data);

//src/camera.c
t_ray	camera_ray(t_camera *cam, int x, int y);

//src/hit.c
int	hit_sphere(t_sphere *sphere, t_ray *ray, double t_min, double t_max, t_hit_record *rec);
int hit_plane(t_plane *plane, t_ray *ray, double t_min, double t_max, t_hit_record *rec);
int	hit_cylinder(t_cylinder *cyl, t_ray *ray, double t_min, double t_max, t_hit_record *rec);

//src/ray.c
t_vec3 ray_at(t_ray ray, double t);

int	hit_scene(t_scene *scene, t_ray *ray, double t_max, t_hit_record *rec);

//lighting.c
t_vec3  shade(t_hit_record *rec, t_scene *scene);

//src/main
void	draw(t_data *data);
