/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_user.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 15:29:57 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/06/28 11:39:41 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int					cl_user(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_cmd;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	free(cmd[0]);
	cmd[0] = ft_strdup("USER");
	new_cmd = cl_command_new(cmd, cl->ncu.chatwin, " 3");
	sv->cmd_list = list_insert_tail(new_cmd, sv->cmd_list);
	return (IS_OK);
}

int					cl_user_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command sends to the server the user needed to log in",
		"if the server needs it.", NULL
	};

	return (cl_help_print(cmd, "<username>", help, cl));
}
