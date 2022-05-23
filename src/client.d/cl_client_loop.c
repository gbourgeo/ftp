/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_client_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/13 08:44:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/01 16:19:56 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/select.h>
#include <unistd.h>
#include <term.h>
#include "cl_main.h"
#include "cl_struct.h"

static int			init_fdset(fd_set *r, fd_set *w, t_client *cl)
{
	t_server	*sv;
	int			max;

	sv = cl->server_list;
	max = 0;
	FD_ZERO(r);
	FD_ZERO(w);
	FD_SET(STDIN_FILENO, r);
	while (sv)
	{
		if (sv->fd_ctrl > 0)
		{
			FD_SET(sv->fd_ctrl, r);
			if (sv->wr.len != 0)
				FD_SET(sv->fd_ctrl, w);
			if (sv->fd_ctrl > max)
				max = sv->fd_ctrl;
		}
		if (sv->fd_data > 0)
		{
			if (sv->cmd_list && sv->cmd_list->data_socket_state != DATA_SOCKET_NOOPERATION)
				FD_SET(sv->fd_data, (sv->cmd_list->data_socket_state == DATA_SOCKET_RECEIVE) ? r : w);
			if (sv->fd_data > max)
				max = sv->fd_data;
		}
		sv = (void *)sv->list.next;
	}
	return (max);
}

static void			check_fdset(fd_set *r, fd_set *w, t_client *cl)
{
	t_server	*sv;

	sv = cl->server_list;
	if (FD_ISSET(STDIN_FILENO, r))
		cl->errnb[0] = cl_ncurses_read(&cl->rd, cl);
	cl->errnb[1] = cl_command_exec_cl(cl);
	while (sv)
	{
		if (sv->fd_ctrl > 0)
		{
			if (FD_ISSET(sv->fd_ctrl, r))
			{
				sv->errnb[0] = cl_server_recv(&sv->rd, sv->fd_ctrl);
			}
			sv->errnb[1] = cl_ncurses_write(&sv->rd, sv, cl);
			if (FD_ISSET(sv->fd_ctrl, w))
			{
				sv->errnb[2] = cl_server_send(&sv->wr, sv->fd_ctrl);
			}
			sv->errnb[3] = cl_command_exec_sv(sv, cl);
		}
		if (sv->fd_data > 0)
		{
			if (FD_ISSET(sv->fd_data, r))
				sv->errnb[5] = cl_server_recv_data(sv, cl);
			if (FD_ISSET(sv->fd_data, w))
				sv->errnb[6] = cl_server_send_data(sv, cl);
		}
		sv = (void *)sv->list.next;
	}
}

static int			check_client_errors(t_client *cl)
{
	int		i;

	i = 0;
	while (i < (int)(sizeof(cl->errnb) / sizeof(cl->errnb[0])))
	{
		if (cl->errnb[i] == ERR_QUIT)
			return (ERR_QUIT);
		if (cl->errnb[i] == IS_OK)
		{
			wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_GREEN));
			wprintw(cl->ncu.chatwin, "SUCCESS cl[%d]\n", i);
			wattroff(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_GREEN));
			wrefresh(cl->ncu.chatwin);
			cl->errnb[i] = NOT_DEFINED;
			wmove(cl->ncu.textwin, 0, 0);
			wrefresh(cl->ncu.textwin);
		}
		else if (cl->errnb[i] != NOT_DEFINED)
		{
			wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_RED));
			wprintw(cl->ncu.chatwin, "ERROR");
			wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BLUE));
			wprintw(cl->ncu.chatwin, " : %s\n", ft_get_error(cl->errnb[i]));
			wattroff(cl->ncu.chatwin, COLOR_PAIR(CL_BLUE));
			wrefresh(cl->ncu.chatwin);
			cl->errnb[i] = NOT_DEFINED;
			wmove(cl->ncu.textwin, 0, 0);
			wrefresh(cl->ncu.textwin);
		}
		i++;
	}
	return (IS_OK);
}

static int			check_servers_errors(t_client * cl)
{
	int			i;
	t_server	*server;
	t_server	*next;

	server = cl->server_list;
	while (server)
	{
		i = 0;
		while (i < (int)(sizeof(server->errnb) / sizeof(server->errnb[0])))
		{
			if (server->errnb[i] == IS_OK)
			{
				wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_GREEN));
				wprintw(cl->ncu.chatwin, "SUCCESS sv[%d]\n", i);
				wattroff(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_GREEN));
				wrefresh(cl->ncu.chatwin);
				cl->errnb[i] = NOT_DEFINED;
				wmove(cl->ncu.textwin, 0, 0);
				wrefresh(cl->ncu.textwin);
				server->errnb[i] = NOT_DEFINED;
			}
			if (server->errnb[i] != NOT_DEFINED)
			{
				ft_strcpy(server->response, "500 Error\n");
				wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BACK_RED));
				wprintw(cl->ncu.chatwin, "ERROR");
				wattron(cl->ncu.chatwin, COLOR_PAIR(CL_BLUE));
				wprintw(cl->ncu.chatwin, " : %s\n", ft_get_error(server->errnb[i]));
				wattroff(cl->ncu.chatwin, COLOR_PAIR(CL_BLUE));
				wrefresh(cl->ncu.chatwin);
				if (server->errnb[i] == ERR_DISCONNECT)
				{
					next = (void *)server->list.next;
					cl->server_list = cl_server_close(server, cl->server_list);
					server = next;
					break ;
				}
				server->errnb[i] = NOT_DEFINED;
				wmove(cl->ncu.textwin, 0, 0);
				wrefresh(cl->ncu.textwin);
			}
			i++;
		}
		if (server)
			server = (void *)server->list.next;
	}
	return (IS_OK);
}

int					cl_client_loop(t_client *cl)
{
	struct timeval	timeout;
	fd_set			fds[2];
	int				max;
	int				ret;

	wmove(cl->ncu.chatwin, 0, 0);
	wprintw(cl->ncu.chatwin, "Welcome to GBO FTP Client !\n");
	wprintw(cl->ncu.chatwin, "Type '?' for a list of available commands.\n\n");
	wrefresh(cl->ncu.chatwin);
	while (check_client_errors(cl) == IS_OK && check_servers_errors(cl) == IS_OK)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;
		max = init_fdset(&fds[0], &fds[1], cl);
		ret = select(max + 1, &fds[0], &fds[1], NULL, &timeout);
		if (ret < 0)
			return (ERR_SELECT);
		check_fdset(&fds[0], &fds[1], cl);
		wmove(cl->ncu.textwin, getcury(cl->ncu.textwin), cl->rd.tail - cl->rd.buff);
		wrefresh(cl->ncu.textwin);
	}
	return (ERR_QUIT);
}
