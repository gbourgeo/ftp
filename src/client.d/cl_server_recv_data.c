/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_server_recv_data.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 02:21:30 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/02 09:56:59 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "cl_main.h"

static int		cl_recv_error(t_server *sv, int ret_value, int err_code)
{
	if (errno == EAGAIN || errno == EWOULDBLOCK)
		return (IS_OK);
	cl_server_close_data(sv);
	return ((ret_value < 0) ? err_code : IS_OK);
}

int				cl_server_recv_data(t_server *sv, t_client *cl)
{
	char		buff[DATA_BUFF_SIZE];
	int			len;
	int			ret;

	len = recv(sv->fd_data, buff, sizeof(buff),
		MSG_DONTWAIT | MSG_NOSIGNAL);
	if (len <= 0)
		return (cl_recv_error(sv, len, ERR_RECV));
	if (sv->filename != NULL)
	{
		wprintw(cl->ncu.chatwin, "Opening file : %s\n", sv->filename);
		wrefresh(cl->ncu.chatwin);
		if (sv->filefd < 0
		&& (sv->filefd = open(sv->filename, O_CREAT | O_TRUNC | O_WRONLY,
			0644)) < 0)
			return (cl_recv_error(sv, -1, ERR_OPEN));
		ret = (write(sv->filefd, buff, len) != len) ? ERR : OK;
	}
	else
	{
		ret = wprintw(cl->printtowin, "%.*s", len, buff);
		wrefresh(cl->printtowin);
	}
	return ((ret == OK) ? IS_OK : ERR_WRITE);
}
