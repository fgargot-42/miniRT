/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_keep_empty.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:50:40 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/22 20:05:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	strcountsub(const char *s, char sep)
{
	int	count;

	count = 1;
	if (!s || !*s)
		return (0);
	while (*s)
	{
		if (*s == sep)
			count++;
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

static char	*putnextstr(char **arr, int i, const char *str, char c)
{
	int	countnext;

	countnext = 0;
	while (str[countnext] && str[countnext] != c)
		countnext++;
	arr[i] = malloc(sizeof(char) * (countnext + 1));
	if (!arr[i])
	{
		freeall(arr);
		return (NULL);
	}
	ft_strlcpy(arr[i], str, countnext + 1);
	if (str[countnext] == c)
		countnext++;
	return ((char *)&str[countnext]);
}

char	**ft_split_keep_empty(char const *s, char c)
{
	char	**strs;
	int		strcount;
	int		i;

	if (!s)
		return (NULL);
	strcount = strcountsub(s, c);
	strs = malloc(sizeof (char *) * (strcount + 1));
	if (!strs)
		return (NULL);
	strs[strcount] = NULL;
	i = 0;
	while (i < strcount && *s)
	{
		s = putnextstr(strs, i, s, c);
		if (!s)
			return (NULL);
		i++;
	}
	return (strs);
}
