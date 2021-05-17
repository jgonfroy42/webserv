/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 10:52:57 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/19 13:01:55 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	recurrence(t_list *lst, void (*del)(void *))
{
	if (lst)
	{
		recurrence(lst->next, (*del));
		(*del)(lst->content);
		free(lst);
	}
}

void		ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	tmp = *lst;
	if (tmp)
		recurrence(tmp, (*del));
	*lst = NULL;
}
