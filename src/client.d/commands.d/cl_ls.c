/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:18:08 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/06/28 11:39:20 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int				cl_ls(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	t_cmd_l	*new_elem;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	new_list = cl_command_new((char *[]){ "PASV", NULL }, cl->ncu.chatwin, " 2");
	free(cmd[0]);
	cmd[0] = ft_strdup("LIST");
	new_elem = cl_command_new(cmd, cl->printtowin, " 2");
	new_elem->data_socket_state = DATA_SOCKET_RECEIVE;
	new_list = elem_insert_tail(new_elem, new_list);
	sv->cmd_list = list_insert_tail(new_list, sv->cmd_list);
	return (IS_OK);
}

int				cl_ls_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command list the current directory filesystem or the",
		"file / directory as first argument.", NULL
	};

	return (cl_help_print(cmd, "[<file_name / directory_name>]", help, cl));
}
