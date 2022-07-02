/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_server_write.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 21:08:30 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/06/28 11:42:36 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int		cl_server_write(const char buf[], t_server *sv)
{
	int		i;
	int		errnb;

	i = 0;
	if (sv == NULL || sv->fd_ctrl < 0)
		return (ERR_NO_SERVER);
	while (buf[i])
	{
		*sv->wr.tail++ = buf[i++];
		if (sv->wr.tail >= sv->wr.buff + sizeof(sv->wr.buff))
			sv->wr.tail = sv->wr.buff;
		if (sv->wr.len++ >= (int)sizeof(sv->wr.buff))
			if ((errnb = cl_server_send(&sv->wr, sv->fd_ctrl)) != IS_OK)
				return (errnb);
	}
	return (IS_OK);
}
