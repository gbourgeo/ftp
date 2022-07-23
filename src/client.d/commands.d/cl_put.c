/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:18:43 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/23 12:13:11 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "cl_main.h"

static int		cl_put_get_dest_path(char **cmd)
{
	char	*filename;
	char	*new;

	filename = ft_strrchr(cmd[1], '/');
	filename = (filename == NULL) ? cmd[1] : filename + 1;
	new = (char *)ft_memalloc(ft_strlen(filename) + ft_strlen(cmd[2]) + 2);
	if (new == NULL)
		return (ERR_MALLOC);
	if (cmd[2])
	{
		ft_strcpy(new, cmd[2]);
		if (new[ft_strlen(new) - 1] != '/')
			ft_strcat(new, "/");
	}
	ft_strcat(new, filename);
	ft_strdel(&cmd[1]);
	cmd[1] = new;
	return (IS_OK);
}

int				cl_put(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	t_cmd_l	*new_elem;
	int		errnb;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	if (cmd[1] == NULL)
		return (ERR_NB_PARAMS);
	sv->filefd = open(cmd[1], O_RDONLY);
	if (sv->filefd < 0)
		return (ERR_OPEN);
	errnb = cl_put_get_dest_path(cmd);
	if (errnb != IS_OK)
		return (errnb);
	ft_strdel(&cmd[0]);
	cmd[0] = ft_strdup("STOR");
	ft_strdel(&cmd[2]);
	new_list = cl_command_new((char *[]){ "PASV", NULL }, cl->ncu.chatwin, " 2");
	new_elem = cl_command_new(cmd, cl->printtowin, " 12");
	new_elem->data_socket_state = DATA_SOCKET_SEND;
	new_list = elem_insert_tail(new_elem, new_list);
	errnb = cl_refresh_server_list_window(new_list, sv, cl);
	return (errnb);
}

int				cl_put_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the user to send a file to the server.", NULL
	};

	return (cl_help_print(cmd, "<filename / filepath> [destination path]", help, cl));
}
