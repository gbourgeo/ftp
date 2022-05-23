/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_server_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 23:54:20 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/13 23:05:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

t_server * cl_server_new(const char *addr, const char *port)
{
	t_server	*sv;
	size_t		i;

	i = 0;;
	sv = ft_memalloc(sizeof(*sv));
	if (sv != NULL)
	{
		sv->address = ft_strdup(addr);
		sv->port = ft_strdup(port);
		sv->fd_ctrl = -1;
		sv->fd_data = -1;
		sv->filefd = -1;
		sv->rd.head = sv->rd.buff;
		sv->rd.tail = sv->rd.buff;
		sv->wr.head = sv->wr.buff;
		sv->wr.tail = sv->wr.buff;
		while (i < sizeof(sv->errnb) / sizeof(sv->errnb[0]))
			sv->errnb[i++] = NOT_DEFINED;
	}
	return (sv);
}