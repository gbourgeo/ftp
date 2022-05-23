/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lists_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 10:40:36 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/22 19:06:52 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lists.h"

void		*list_insert_head(void *elem, void *list)
{
	((t_clist *)elem)->next = list;
	if (list != 0)
		((t_clist *)list)->prev = elem;
	return (elem);
}

void		*list_insert_tail(void *elem, void *list)
{
	t_clist	*ptr;

	ptr = (t_clist *)list;
	if (ptr == (void *)0)
		return (elem);
	while (ptr->next != (void *)0)
		ptr = ptr->next;
	ptr->next = elem;
	((t_clist *)elem)->prev = ptr;
	return (list);
}

void		*elem_insert_head(void *elem, void *list)
{
	((t_celem *)elem)->elem.next = list;
	if (list != 0)
		((t_celem *)list)->elem.prev = elem;
	return (elem);
}

void		*elem_insert_tail(void *elem, void *list)
{
	t_celem	*ptr;

	ptr = (t_celem *)list;
	if (ptr == (void *)0)
		return (elem);
	while (ptr->elem.next != (void *)0)
		ptr = (t_celem *)ptr->elem.next;
	ptr->elem.next = elem;
	((t_celem *)elem)->elem.prev = (t_clist *)ptr;
	return (list);
}

