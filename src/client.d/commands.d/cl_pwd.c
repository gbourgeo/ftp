/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:19:02 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/17 17:24:06 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int				cl_pwd(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_cmd;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	free(cmd[0]);
	cmd[0] = ft_strdup("PWD");
	new_cmd = cl_command_new(cmd, cl->ncu.chatwin, " ");
	sv->cmd_list = list_insert_tail(new_cmd, sv->cmd_list);
	return (NOT_DEFINED);
}

int				cl_pwd_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command prints the current working directory on the",
		"server.", NULL
	};

	return (cl_help_print(cmd, "", help, cl));
}
