/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfilter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 23:18:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 18:46:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstfilter(t_list *lst, int (*filter)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	if (!filter || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		if (filter(lst->content))
		{
			new_node = ft_lstnew(lst->content);
			if (!new_node)
			{
				ft_lstclear(&new_list, del);
				return (NULL);
			}
			ft_lstadd_back(&new_list, new_node);
		}
		lst = lst->next;
	}
	return (new_list);
}
