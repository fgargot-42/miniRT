/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_by_whitespace.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:19:36 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 19:55:01 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	strcountsub(const char *s)
{
	int	count;
	int	issep;

	count = (*s != '\0');
	issep = 1;
	if (!s)
		return (0);
	while (*s)
	{
		if (ft_iswhitespace(*s) && issep)
		{
			count++;
			issep = 0;
		}
		if (ft_iswhitespace(*s))
			issep = 1;
		s++;
	}
	return (count);
}

static void	freeall(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static char	*putnextstr(char **arr, int i, const char *str)
{
	int	countnext;

	while (*str && ft_iswhitespace(*str))
		str++;
	countnext = 0;
	while (str[countnext] && !ft_iswhitespace(str[countnext]))
		countnext++;
	arr[i] = malloc(sizeof(char) * (countnext + 1));
	if (!arr[i])
	{
		freeall(arr);
		return (NULL);
	}
	ft_strlcpy(arr[i], str, countnext + 1);
	return ((char *)&str[countnext]);
}

char	**ft_split_by_whitespace(char const *s)
{
	char	**strs;
	int		strcount;
	int		i;

	if (!s)
		return (NULL);
	strcount = strcountsub(s);
	strs = ft_calloc(sizeof (char *), (strcount + 1));
	if (!strs)
		return (NULL);
	i = 0;
	while (i < strcount && *s)
	{
		s = putnextstr(strs, i, s);
		if (!s)
			return (NULL);
		i++;
	}
	return (strs);
}
