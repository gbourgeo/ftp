/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_ncurses_keys.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 22:09:20 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/21 14:19:31 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int			cl_ctrl_c(t_buff *ring, t_client *cl)
{
	ring->head = ring->buff;
	ring->tail = ring->buff;
	ring->len = 0;
	ft_strclr(ring->buff);
	wclear(cl->ncu.textwin);
	wrefresh(cl->ncu.textwin);
	return (NOT_DEFINED);
}

int			cl_ctrl_d(t_buff *ring, t_client *cl)
{
	(void)cl;
	if (ring->len == 0)
		return (ERR_QUIT);
	return (NOT_DEFINED);
}

int			cl_lf(t_buff *ring, t_client *cl)
{
	int		errnb;
	int		verbose;

	errnb = NOT_DEFINED;
	if (ring->len > 0)
	{
		ring->tail = ring->buff + ring->len;
		*ring->tail++ = '\n';
		*ring->tail++ = '\0';
		verbose = GET_BIT(cl->options, cl_verbose);
		SET_BIT(cl->options, cl_verbose, 1);
		cl->printtowin = cl->ncu.chatwin;
		errnb = cl_client_commands(ring->head, cl->server_list, cl);
		SET_BIT(cl->options, cl_verbose, verbose);
		*(ring->tail - 2) = '\0';
		ring->head = ring->buff;
		ring->tail = ring->buff;
		ring->len = 0;
		cl->hist = cl_history_add(ring->buff, cl->hist);
		ft_strclr(ring->buff);
		werase(cl->ncu.textwin);
		wrefresh(cl->ncu.textwin);
	}
	return (errnb);
}

int			cl_backspace(t_buff *ring, t_client *cl)
{
	if (ring->tail > ring->buff && ring->len > 0)
	{
		ring->tail--;
		ft_strcpy(ring->tail, ring->tail + 1);
		ring->len--;
		mvwdelch(cl->ncu.textwin, getcury(cl->ncu.textwin),
		getcurx(cl->ncu.textwin) - 1);
		wrefresh(cl->ncu.textwin);
	}
	return (NOT_DEFINED);
}

int			cl_key_dc(t_buff *ring, t_client *cl)
{
	if (*ring->tail)
	{
		ft_strcpy(ring->tail, ring->tail + 1);
		ring->len--;
		wdelch(cl->ncu.textwin);
		wrefresh(cl->ncu.textwin);
	}
	return (NOT_DEFINED);
}
