/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_ncurses_write.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 21:05:21 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/02 08:31:04 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "cl_main.h"

static void		cl_get_response(char *stop, t_buff *ring, t_server *sv,
t_client *cl)
{
	int			i;

	(void)cl;
	i = 0;
	while (ring->head != stop && i < (int)sizeof(sv->response) - 1)
	{
		sv->response[i++] = *ring->head++;
		if (ring->head >= ring->buff + sizeof(ring->buff))
			ring->head = ring->buff;
	}
	sv->response[i] = '\0';
}

static void		cl_check_response(t_server *sv, t_client *cl)
{
	char	min_response;

	min_response = (sv->cmd_list == NULL) ? '2' : sv->cmd_list->ret_codes[0];
	if (is_valid_response(sv->response))
	{
		if (sv->response[0] - min_response > 0)
		{
			wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_RED));
			wprintw(cl->ncu.chatwin, "ERROR");
			wattroff(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_RED));
		}
		else
		{
			wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_GREEN));
			wprintw(cl->ncu.chatwin, "SUCCESS");
			wattroff(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_GREEN));
		}
	}
	wprintw(cl->ncu.chatwin, " %s", sv->response);
	wrefresh(cl->ncu.chatwin);
	wmove(cl->ncu.textwin, 0, 0);
	wrefresh(cl->ncu.textwin);
}

int				cl_ncurses_write(t_buff *ring, t_server *sv, t_client *cl)
{
	char	*ptr;
	int		i;
	int		errnb;

	ptr = ring->head;
	i = (ring->tail >= ring->buff + sizeof(ring->buff)) ? 1 : 0;
	errnb = IS_OK;
	if (ring->tail != ring->head)
		while (ptr != ring->tail - i)
		{
			if (*ptr++ == '\n')
			{
				cl_get_response(ptr, ring, sv, cl);
				cl_check_response(sv, cl);
				errnb = IS_OK;
			}	
			if (ptr >= ring->buff + sizeof(ring->buff))
				ptr = ring->buff;
		}
	if (ring->tail >= ring->buff + sizeof(ring->buff))
		ring->tail = ring->buff;
	if (ring->tail >= ring->head)
		ring->len = (ring->buff + sizeof(ring->buff)) - ring->tail;
	else
		ring->len = ring->head - ring->tail - 1;
	return (errnb);
}
