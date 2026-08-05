/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ptrtoa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 19:02:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/05 19:02:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_ptrtoa(void *p)
{
	unsigned long	ptrnb;
	char			*s;
	char			c;
	size_t			i;

	ptrnb = (unsigned long)p;
	s = malloc(sizeof(char) * (sizeof(ptrnb) * 2 + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (i < sizeof(ptrnb) * 2)
	{
		c = ptrnb >> (((sizeof(ptrnb) * 2) - 1 - i) * 4) & 0xf;
		if (c >= 10)
			c += 'a' - 10;
		else
			c += '0';
		s[i] = c;
		i++;
	}
	s[i] = 0;
	return (s);
}
