/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_server_send.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 18:51:48 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/14 16:40:23 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "cl_main.h"

static int	cl_send_error(int errnb)
{
	if (errnb == 0 || errno == ECONNRESET || errno == EPIPE)
		return (ERR_DISCONNECT);
	if (errno == EAGAIN || errno == EWOULDBLOCK)
		return (NOT_DEFINED);
	return (ERR_SEND);
}

/*
** Function to send commands to the Server.
*/
int			cl_server_send(t_buff *ring, int fd)
{
	int		ret;

	if (ring->len == 0)
		return (NOT_DEFINED);
	if (ring->head < ring->tail)
		ret = send(fd, ring->head, ring->tail - ring->head,
		MSG_DONTWAIT | MSG_NOSIGNAL);
	else
		ret = send(fd, ring->head,
		ring->buff + sizeof(ring->buff) - ring->head,
		MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret <= 0)
		return (cl_send_error(ret));
	if ((ring->head += ret) >= ring->buff + sizeof(ring->buff))
		ring->head = ring->buff;
	ring->len -= ret;
	return (NOT_DEFINED);
}
