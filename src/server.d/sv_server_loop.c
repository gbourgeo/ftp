/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_server_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/13 08:45:52 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/10/16 23:38:12 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/select.h>
#include <time.h>
#include "sv_main.h"

static void		sv_check_clients(t_client *cl, t_server *sv)
{
	int			i;

	while (cl)
	{
		i = 0;
		if (cl->data.pid > 0)
			cl->errnb[3] = sv_check_pid(cl, sv);
		while (i < (int)(sizeof(cl->errnb) / sizeof(cl->errnb[0])))
			if (cl->errnb[i] != IS_OK)
			{
				cl = sv_client_end(cl, sv);
				break ;
			}
			else
				i++;
		if (i == (int)(sizeof(cl->errnb) / sizeof(cl->errnb[0])))
			cl = cl->next;
	}
}

static int		sv_check_and_set_fd(t_data *data, int max, fd_set *fdr)
{
	if (data->pasv_fd > 0)
		FD_SET(data->pasv_fd, fdr);
	if (data->pasv_fd > max)
		max = data->pasv_fd;
	// if (data->sock_fd > 0)
	// 	FD_SET(data->sock_fd, fdr);
	// if (data->sock_fd > max)
	// 	max = data->sock_fd;
	return (max);
}

static int		sv_init_fd(fd_set *fdr, fd_set *fdw, t_server *sv)
{
	t_client	*cl;
	int			max;

	cl = sv->clients;
	max = (sv->ip[sv_v4] > sv->ip[sv_v6]) ? sv->ip[sv_v4] : sv->ip[sv_v6];
	FD_ZERO(fdr);
	FD_ZERO(fdw);
	if (sv->ip[sv_v4] > 0)
		FD_SET(sv->ip[sv_v4], fdr);
	if (sv->ip[sv_v6] > 0)
		FD_SET(sv->ip[sv_v6], fdr);
	while (cl)
	{
		FD_SET(cl->fd, fdr);
		if (cl->wr.len > 0)
			FD_SET(cl->fd, fdw);
		if (cl->fd > max)
			max = cl->fd;
		max = sv_check_and_set_fd(&cl->data, max, fdr);
		cl = cl->next;
	}
	return (max);
}

static void		sv_check_fd(int ret, fd_set *fdr, fd_set *fdw, t_server *sv)
{
	t_client	*cl;

	cl = sv->clients;
	if (sv->ip[sv_v4] > 0 && FD_ISSET(sv->ip[sv_v4], fdr) && ret-- > 0)
		if ((sv->errnb[0] = sv_server_accept(sv_v4, sv)) != IS_OK)
			sv_server_close(sv_v4, 1, sv);
	if (sv->ip[sv_v6] > 0 && FD_ISSET(sv->ip[sv_v6], fdr) && ret-- > 0)
		if ((sv->errnb[1] = sv_server_accept(sv_v6, sv)) != IS_OK)
			sv_server_close(sv_v6, 1, sv);
	while (cl && ret > 0)
	{
		if (FD_ISSET(cl->fd, fdr) && ret-- > 0)
			cl->errnb[0] = sv_client_recv(cl, sv);
		if (FD_ISSET(cl->fd, fdw) && ret-- > 0)
			cl->errnb[1] = sv_client_send(cl);
		if (cl->data.pasv_fd > 0
		&& FD_ISSET(cl->data.pasv_fd, fdr) && ret-- > 0)
			cl->errnb[2] = sv_listen_from(cl, sv);
		cl = cl->next;
	}
}

int				sv_server_loop(t_server *sv)
{
	int				max;
	int				ret;
	fd_set			fd_read;
	fd_set			fd_write;
	struct timeval	timeout;

	if (GET_BIT(sv->options, sv_interactive))
		sv_server_info(sv);
	while (sv->ip[sv_v4] > 0 || sv->ip[sv_v6] > 0)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		sv_check_clients(sv->clients, sv);
		max = sv_init_fd(&fd_read, &fd_write, sv);
		ret = select(max + 1, &fd_read, &fd_write, NULL, &timeout);
		if (ret < 0)
			return (ERR_SELECT);
		if (ret > 0)
			sv_check_fd(ret, &fd_read, &fd_write, sv);
	}
	return (IS_OK);
}
