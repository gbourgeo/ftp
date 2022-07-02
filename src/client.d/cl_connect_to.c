/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_connect_to.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/05 23:31:35 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/06/28 11:42:22 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __linux__
# define _XOPEN_SOURCE 700
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "cl_main.h"

static int	init_connection(struct addrinfo **res, char *addr, char *port)
{
	struct addrinfo hints;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(addr, port, &hints, res))
		return (ERR_GETADDR);
	return (IS_OK);
}

int			cl_connect_to(int *fd, char *addr, char *port, int *ip_version)
{
	struct addrinfo *results;
	struct addrinfo *tmp;
	int errnb;
	int on;

	if ((errnb = init_connection(&results, addr, port)) != IS_OK)
		return (errnb);
	tmp = results;
	while (tmp)
	{
		*fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (*fd > 0
		&& connect(*fd, tmp->ai_addr, tmp->ai_addrlen) == 0)
			break;
		ft_close(fd);
		tmp = tmp->ai_next;
	}
	if (*fd > 0 && tmp && ip_version)
		*ip_version = tmp->ai_family;
	freeaddrinfo(results);
	if (*fd < 0)
		return (ERR_NO_SERVER);
	on = 1;
	if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
		return (ERR_SETSOCKOPT);
	return (IS_OK);
}

int 		cl_connect_to_server(t_server *sv, t_client *cl)
{
	int errnb;

	if (GET_BIT(cl->options, cl_verbose))
	{
		wprintw(cl->ncu.chatwin, "Connection to ");
		wattron(cl->ncu.chatwin, A_UNDERLINE);
		wprintw(cl->ncu.chatwin, "%s:%s", sv->address, sv->port);
		wattroff(cl->ncu.chatwin, A_UNDERLINE);
		wprintw(cl->ncu.chatwin, " ");
		wrefresh(cl->ncu.chatwin);
	}
	errnb = cl_connect_to(&sv->fd_ctrl, sv->address, sv->port, &sv->ip_version);
	if (GET_BIT(cl->options, cl_verbose))
	{
		wattron(cl->ncu.chatwin, (errnb == IS_OK) ?
			COLOR_PAIR(CL_GREEN) : COLOR_PAIR(CL_RED));
		wprintw(cl->ncu.chatwin, (errnb == IS_OK) ?
			"OK\n" : "KO\n");
		wattroff(cl->ncu.chatwin, (errnb == IS_OK) ?
			COLOR_PAIR(CL_GREEN) : COLOR_PAIR(CL_RED));
		wrefresh(cl->ncu.chatwin);
	}
	return (errnb);
}
