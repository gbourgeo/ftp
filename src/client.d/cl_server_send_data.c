/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_server_send_data.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 18:16:40 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/02 09:57:45 by gbourgeo         ###   ########.fr       */
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

static int		cl_send_error(t_server *sv, int ret_value, int err_code)
{
	if (errno == EAGAIN || errno == EWOULDBLOCK)
	{
		nanosleep(&(struct timespec){ 0, 500000L }, NULL);
		return (IS_OK);
	}
	cl_server_close_data(sv);
	return ((ret_value < 0) ? err_code : IS_OK);
}

int				cl_server_send_data(t_server *sv, t_client *cl)
{
	char		buff[DATA_BUFF_SIZE];
	int			bytes_read;
	int			bytes_send;

	(void)cl;
	errno = 0;
	bytes_read = read(sv->filefd, buff, sizeof(buff));
	if (bytes_read <= 0)
		return (cl_send_error(sv, bytes_read, ERR_READ));
	errno = 0;
	bytes_send = send(sv->fd_data, buff, bytes_read, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (bytes_send <= 0)
		return (cl_send_error(sv, bytes_send, ERR_SEND));
	if (bytes_send != bytes_read)
		return (cl_send_error(sv, -1, ERR_BYTES_SENT));
	return (IS_OK);
}
