/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_refresh.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 00:03:57 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/23 16:40:03 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int					cl_refresh_server_list_window(t_cmd_l *new_list,
t_server *sv, t_client *cl)
{
	t_cmd_l	*new_elem;

	new_elem = cl_command_new((char *[]){ "PASV", NULL }, cl->ncu.chatwin, "2 ");
	new_list = elem_insert_tail(new_elem, new_list);
	new_elem = cl_command_new((char *[]){ "NLST", NULL }, cl->ncu.slistwin, "2 2");
	new_elem->data_socket_state = DATA_SOCKET_RECEIVE;
	new_list = elem_insert_tail(new_elem, new_list);
	sv->cmd_list = list_insert_tail(new_list, sv->cmd_list);
	return (NOT_DEFINED);
}

int					cl_refresh(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_cmd;

	(void)cmd;
	wprintw(cl->ncu.chatwin, "Refreshing...\n");
	wrefresh(cl->ncu.chatwin);
	new_cmd = cl_command_new((char *[]){ "\\ls -ap", NULL }, cl->ncu.clistwin, " ");
	cl->cmd_list = list_insert_tail(new_cmd, cl->cmd_list);
	new_cmd = cl_command_new((char *[]){ "NLST", NULL }, cl->ncu.slistwin, " ");
	new_cmd->sv = sv;
	cl->cmd_list = list_insert_tail(new_cmd, cl->cmd_list);
	return (NOT_DEFINED);
}

int					cl_refresh_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the client to refresh the \"Server List\"",
		"Window and the \"Client List\" Window.", NULL
	};

	return (cl_help_print(cmd, "", help, cl));
}
