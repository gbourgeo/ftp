/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_connect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 02:24:07 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/01 16:13:38 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int					cl_connect(char **cmd, t_server *sv, t_client *cl)
{
	t_server	*new_server;
	t_cmd_l		*new_cmd;
	int			ret;

	(void)sv;
	if (!cmd[1] || !cmd[2])
		return (ERR_NB_PARAMS);
	new_server = cl_server_new(cmd[1], cmd[2]);
	cl->server_list = list_insert_head(new_server, cl->server_list);
	// ret = cl_get_userinfo(new_server, cl);
	ret = cl_connect_to_server(new_server, cl);
	if (ret != NOT_DEFINED)
		cl->server_list = cl_server_close(new_server, cl->server_list);
	else
	{
		new_cmd = cl_command_new((char *[]){ "refresh", NULL }, cl->ncu.chatwin, "2 " );
		new_cmd->sv = new_server;
		cl->cmd_list = elem_insert_tail(new_cmd, cl->cmd_list);
	}
	return (ret);
}

int					cl_connect_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the client to connect to a server",
		"remotly.", NULL
	};

	return (cl_help_print(cmd, "<address> <port>", help, cl));
}
