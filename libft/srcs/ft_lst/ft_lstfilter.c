/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfilter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 23:18:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/05 14:48:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstfilter(t_list *lst, int (*filter)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	if (!filter)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		if (filter(lst->content))
		{
			new_node = ft_lstnew(lst->content);
			if (!new_node)
			{
				ft_lstclear(&new_list, NULL);
				return (NULL);
			}
			ft_lstadd_back(&new_list, new_node);
		}
		lst = lst->next;
	}
	return (new_list);
}
