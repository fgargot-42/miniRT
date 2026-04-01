/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   veclib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:48:36 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/02 18:52:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "math.h"

t_vec3 vec_add(t_vec3 a, t_vec3 b)
{
    return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}
t_vec3 vec_sub(t_vec3 a, t_vec3 b)
{
    return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}
t_vec3 vec_scale(t_vec3 v, double k)
{
    return (t_vec3){v.x * k, v.y * k, v.z * k};
}
double vec_dot(t_vec3 a, t_vec3 b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}
t_vec3 vec_cross(t_vec3 a, t_vec3 b)
{
    return (t_vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
double vec_length(t_vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
t_vec3 vec_normalize(t_vec3 v)
{
    double len = vec_length(v);
    if (len == 0)
        return (t_vec3){0, 0, 0};
    return (t_vec3){v.x / len, v.y / len, v.z / len};
}

// Component-wise multiplication (for colors)
t_vec3 vec_multiply(t_vec3 a, t_vec3 b)
{
    return (t_vec3){a.x * b.x, a.y * b.y, a.z * b.z};
}

t_vec3 vec_clamp(t_vec3 v, double min, double max)
{
    return (t_vec3){
        fmax(min, fmin(max, v.x)),
        fmax(min, fmin(max, v.y)),
        fmax(min, fmin(max, v.z))
    };
}

double vec_distance(t_vec3 a, t_vec3 b)
{
    return vec_length(vec_sub(b, a));
}

