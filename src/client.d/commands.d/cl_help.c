/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_help.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:17:53 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/06/28 11:39:18 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int			cl_help(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_cmd;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	free(cmd[0]);
	cmd[0] = ft_strdup("HELP");
	new_cmd = cl_command_new(cmd, cl->ncu.chatwin, " 2");
	sv->cmd_list = list_insert_tail(new_cmd, sv->cmd_list);
	return (IS_OK);
}

int			cl_help_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command print the server help.", NULL
	};

	return (cl_help_print(cmd, "[<command_name>]", help, cl));
}
