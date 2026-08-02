/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 16:49:01 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/01 19:17:17 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	joinlen;
	char	*join;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	joinlen = ft_strlen(s1) + ft_strlen(s2);
	join = malloc(sizeof(char) * (joinlen + 1));
	if (!join)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		join[i] = s1[i];
		i++;
	}
	while (*s2)
	{
		join[i] = *s2;
		s2++;
		i++;
	}
	join[i] = '\0';
	return (join);
}

char	*ft_strjoin_array(char const **str_array, char *sep)
{
	size_t	array_size;
	size_t	i;
	char	*join;
	char	*tmp;

	array_size = 0;
	while (str_array[array_size])
		array_size++;
	if (!array_size || !sep)
		return (NULL);
	join = ft_strdup(str_array[0]);
	i = 1;
	while (str_array[i])
	{
		tmp = ft_strjoin(join, sep);
		free(join);
		join = ft_strjoin(tmp, str_array[i]);
		free(tmp);
		i++;
	}
	return (join);
}
