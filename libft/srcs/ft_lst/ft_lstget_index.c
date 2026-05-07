/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstget_index.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 23:20:47 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/08 01:54:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstget_elem_index(t_list *lst, int index)
{
	t_list	*elem;

	if (!lst || index > 0)
		return (NULL);
	elem = lst;
	while (elem && index)
	{
		elem = elem->next;
		index--;
	}
	return (elem);
}
