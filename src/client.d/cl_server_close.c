/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_server_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 18:04:41 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/03 13:31:43 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

t_server *			cl_server_close(t_server *sv, t_server *sv_list)
{
	t_server	*ret;

	ft_strdel(&sv->address);
	ft_strdel(&sv->port);
	ft_strdel(&sv->username);
	ft_strdel(&sv->password);
	ft_close(&sv->fd_ctrl);
	cl_server_close_data(sv);
	ft_strdel(&sv->working_dir);
	while (sv->cmd_list)
		sv->cmd_list = cl_command_remove_list(sv->cmd_list, sv->cmd_list);
	if (sv->list.prev != NULL)
		sv->list.prev->next = sv->list.next;
	if (sv->list.next != NULL)
		sv->list.next->prev = sv->list.prev;
	ret = (sv->list.prev == NULL) ? (void *)sv->list.next : sv_list;
	free(sv);
	return (ret);
}

void				cl_server_close_data(t_server *sv)
{
	ft_close(&sv->fd_data);
	ft_strdel(&sv->filename);
	ft_close(&sv->filefd);
}
