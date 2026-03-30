#include "veclib.h"
#include "mlx.h"
#include "math.h"
#include "SDL2/SDL_scancode.h"
#define WIDTH 800
#define HEIGHT 600

typedef struct s_data
{
	mlx_context	mlx;
	mlx_window	win;
	mlx_image	img;
}	t_data;

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
}	t_camera;


