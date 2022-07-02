/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_server_send_data.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 18:16:40 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/30 13:24:32 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define __USE_POSIX199309
#include <time.h>
#include "cl_main.h"

static int		cl_send_error(t_server *sv)
{
	if (errno == EAGAIN || errno == EWOULDBLOCK)
	{
		nanosleep(&(struct timespec){ 0, 500000L }, NULL);
		return (IS_OK);
	}
	ft_close(&sv->filefd);
	ft_close(&sv->fd_data);
	return ((sv->ret-- < 0) ? ERR_SEND : IS_OK);
}

int				cl_server_send_data(t_server *sv, t_client *cl)
{
	char		buff[DATA_BUFF_SIZE];
	int			bytes_read;

	(void)cl;
	errno = 0;
	sv->ret = read(sv->filefd, buff, sizeof(buff));
	if (sv->ret <= 0)
		return (cl_send_error(sv));
	bytes_read = sv->ret;
	errno = 0;
	sv->ret = send(sv->fd_data, buff, bytes_read, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (sv->ret <= 0)
		return (cl_send_error(sv));
	if (sv->ret != bytes_read && (sv->ret = -1))
		return (cl_send_error(sv));
	return (IS_OK);
}
