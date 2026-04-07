#ifndef VECLIB_H
#define VECLIB_H
typedef struct s_vec3
{
	double x;
	double y;
	double z;
}	t_vec3;


t_vec3 vec_add(t_vec3 a, t_vec3 b);
t_vec3 vec_sub(t_vec3 a, t_vec3 b);
t_vec3 vec_scale(t_vec3 v, double scalar);

// meme direction -> +
// perpendiculaire -> 0
// opposite			-> -
double vec_dot(t_vec3 a, t_vec3 b);

t_vec3 vec_cross(t_vec3 a, t_vec3 b);
double vec_length(t_vec3 v);

t_vec3 vec_normalize(t_vec3 v);

t_vec3 vec_multiply(t_vec3 a, t_vec3 b);
t_vec3 vec_clamp(t_vec3 v, double min, double max);
double vec_distance(t_vec3 a, t_vec3 b);

void	free_matrix(double **matrix);
double	**vec_get_matrix_rotation_z(t_vec3 v_from);
t_vec3	vec_apply_rotation_z(t_vec3 v_from, double **t_matrix);
#endif
