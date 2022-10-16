/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_client_signals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/09 06:26:06 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/10/16 23:27:12 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "cl_main.h"

extern t_client g_client;

static void		cl_timeout(t_client *cl)
{
	time_t		start;
	time_t		end;

	start = time(NULL);
	end = start;
	while (end - start < 5)
	{
		wprintw(cl->ncu.chatwin, "Quitting in %d", 1 - (end - start));
		wrefresh(cl->ncu.chatwin);
		wmove(cl->ncu.chatwin, getcury(cl->ncu.chatwin), 0);
		end = time(NULL);
	}
}

static void		cl_sig_hdlr(int sig)
{
	t_client	*cl;

	cl = &g_client;
	wattron(cl->ncu.chatwin, COLOR_PAIR(CL_RED));
	wprintw(cl->ncu.chatwin, "\n********************");
	wattron(cl->ncu.chatwin, COLOR_PAIR(CLIENT_TEXT_COLOR));
	wprintw(cl->ncu.chatwin, " Signal ");
	wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BLUE));
	wprintw(cl->ncu.chatwin, "%s", get_signal_name(sig));
	wattron(cl->ncu.chatwin, COLOR_PAIR(CLIENT_TEXT_COLOR));
	wprintw(cl->ncu.chatwin, " received ");
	wattron(cl->ncu.chatwin, COLOR_PAIR(CL_RED));
	wprintw(cl->ncu.chatwin, "********************\n");
	wattroff(cl->ncu.chatwin, COLOR_PAIR(CL_RED));
	wrefresh(cl->ncu.chatwin);
	if (sig < 0 || sig > NSIG || cl->sig[sig] == SIG_ERR)
		return ;
	cl_timeout(cl);
	cl_client_end(cl);
	exit(EXIT_FAILURE);
}

int				cl_client_signals(t_client *cl)
{
	int			i;

	i = 0;
	while (i < NSIG)
		cl->sig[i++] = SIG_ERR;
	cl->sig[SIGINT] = signal(SIGINT, cl_sig_hdlr);
	return (IS_OK);
}
