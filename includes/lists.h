/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 10:38:41 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/22 18:38:17 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTS_H
# define LISTS_H

typedef struct	s_chained_list
{
	struct s_chained_list	*prev;
	struct s_chained_list	*next;
}				t_clist;

typedef struct s_chained_elem
{
	t_clist		list;
	t_clist		elem;
}				t_celem;

void		*list_insert_head(void *elem, void *list);
void		*list_insert_tail(void *elem, void *list);
void		*elem_insert_head(void *elem, void *list);
void		*elem_insert_tail(void *elem, void *list);

#endif
