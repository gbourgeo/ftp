/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_client_end.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/11 20:09:46 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/22 18:01:03 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <fcntl.h>
#include "cl_main.h"

void			cl_ncurses_end(t_client *cl)
{
	if (cl->ncu.chatbox)
		delwin(cl->ncu.chatbox);
	if (cl->ncu.chatwin)
		delwin(cl->ncu.chatwin);
	if (cl->ncu.slistbox)
		delwin(cl->ncu.slistbox);
	if (cl->ncu.slistwin)
		delwin(cl->ncu.slistwin);
	if (cl->ncu.clistbox)
		delwin(cl->ncu.clistbox);
	if (cl->ncu.clistwin)
		delwin(cl->ncu.clistwin);
	if (cl->ncu.textbox)
		delwin(cl->ncu.textbox);
	if (cl->ncu.textwin)
		delwin(cl->ncu.textwin);
	if (cl->ncu.main)
		endwin();
	ft_memset(&cl->ncu, 0, sizeof(cl->ncu));
}

void			cl_client_end(t_client *cl)
{
	int		i;

	i = 0;
	ft_strdel(&cl->info.env.path);
	ft_strdel(&cl->info.env.home);
	ft_strdel(&cl->info.env.pwd);
	ft_strdel(&cl->info.env.oldpwd);
	while (i < NSIG)
	{
		if (cl->sig[i] != SIG_ERR)
			signal(i, cl->sig[i]);
		i++;
	}
	cl_ncurses_end(cl);
	while (cl->cmd_list)
		cl->cmd_list = cl_command_remove_list(cl->cmd_list, cl->cmd_list);
	while (cl->server_list)
		cl->server_list = cl_server_close(cl->server_list, cl->server_list);
	cl_history_end(cl->hist);
}

void			cl_history_end(t_hist *hist)
{
	t_hist		*ptr;
	int			fd;

	if (!(ptr = hist))
		return ;
	if ((fd = open(CL_HIST_FILE, O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0)
		return ;
	while (ptr->list.next)
		ptr = (t_hist *)ptr->list.next;
	while (ptr)
	{
		if (ptr->line && *ptr->line)
			ft_putendl_fd(ptr->line, fd);
		ptr = (t_hist *)ptr->list.prev;
	}
	ft_close(&fd);
	while (ptr)
	{
		hist = (t_hist *)ptr->list.next;
		ft_strdel(&ptr->line);
		free(ptr);
		ptr = hist;
	}
}
