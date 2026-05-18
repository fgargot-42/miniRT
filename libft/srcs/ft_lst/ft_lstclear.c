/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:44:18 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/18 16:41:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*node;
	t_list	*next;

	if (!lst)
		return ;
	node = *lst;
	while (node)
	{
		next = node->next;
		if (del)
			del(node->content);
		free(node);
		node = next;
	}
	*lst = NULL;
}
