/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:38:22 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/12 22:56:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "object.h"

static int	get_material_element_index(char *mat_elem)
{
	unsigned long		i;
	static const char	*el_mat[] = {"Ns", "Ka", "Kd", "Ks", "Ke", "Ni", "d"};

	i = 0;
	while (i < sizeof(el_mat) / sizeof(char *))
	{
		if (!ft_strncmp(mat_elem, el_mat[i], ft_strlen(el_mat[i])))
			break ;
		i++;
	}
	if (i >= sizeof(el_mat) / sizeof(char *))
		return (-1);
	return (i);
}

static int	parse_material_line(char *line, t_list **mat_list, int line_nb)
{
	int								index;
	static t_material				*mat;
	static const t_obj_parser_fc	parse_elem[] = {parse_mat_exponent,
		parse_mat_ambient, parse_mat_diffuse, parse_mat_specular,
		parse_mat_emissive, parse_mat_density, parse_mat_opacity};

	if (!line)
	{
		if (mat)
			free(mat);
		mat = NULL;
		return (0);
	}
	if (!ft_strncmp(line, "newmtl", 6))
	{
		index = parse_new_material(line, mat_list, &mat);
		return (index);
	}
	index = get_material_element_index(line);
	if (index != -1)
		index = parse_elem[index](line, mat, line_nb);
	return (index);
}

static int	material_parse_loop(int mtl_fd, t_list **mat_list)
{
	int			line_nb;
	int			status;
	char		*line;

	line = get_next_line(mtl_fd);
	line_nb = 1;
	status = 1;
	while (line && status)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (line[0] && line[0] != '#')
			status = parse_material_line(line, mat_list, line_nb);
		free(line);
		line = get_next_line(mtl_fd);
		line_nb++;
	}
	if (!status)
		clear_gnl(mtl_fd, line);
	return (status);
}

int	import_materials(char *mtl_file, t_list **mat_list, char *rt_path)
{
	int			fd;
	int			status;
	char		**split;

	split = ft_split_by_whitespace(mtl_file);
	if (!split)
		return (0);
	mtl_file = ft_strjoin(rt_path, split[1]);
	fd = open_file_read(mtl_file, "mtl");
	free(mtl_file);
	free_str_array(split);
	if (fd < 0)
		return (0);
	status = material_parse_loop(fd, mat_list);
	return (status);
}
