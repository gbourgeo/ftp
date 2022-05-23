/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_nlst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 02:56:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/18 13:41:25 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int				cl_nlst(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_cmd;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	new_cmd = cl_command_new((char *[]){ "PASV", NULL }, cl->ncu.chatwin, " ");
	sv->cmd_list = list_insert_tail(new_cmd, sv->cmd_list);
	free(cmd[0]);
	cmd[0] = ft_strdup("NLST");
	new_cmd = cl_command_new(cmd, cl->printtowin, "2 2");
	new_cmd->data_socket_state = DATA_SOCKET_RECEIVE;
	sv->cmd_list = list_insert_tail(new_cmd, sv->cmd_list);
	return (NOT_DEFINED);
}

int				cl_nlst_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command prints a short listing of the current directory or",
		"the file / directory pointed as first argument on the server.",
		" This command is usefull for a complete get or put of a",
		"directory.", NULL
	};

	return (cl_help_print(cmd, "[<file_name / directory_name>]", help, cl));
}
