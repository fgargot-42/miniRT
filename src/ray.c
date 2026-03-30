#include "miniRT.h"

t_vec3 ray_at(t_ray ray, double t)
{
    return vec_add(ray.origin, vec_scale(ray.direction, t));
}
